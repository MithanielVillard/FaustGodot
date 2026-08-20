#include "AudioEffectFaust.h"
#include "GodotMapUI.h"

#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/classes/audio_server.hpp>

using namespace godot;

AudioEffectFaust::AudioEffectFaust()
{
    m_dspUI = std::make_unique<GodotMapUI>(*this);
    m_midiHandler = std::make_unique<GodotMidi>();
    m_midiUI      = std::make_unique<MidiUI>(m_midiHandler.get());

    m_midiHandler->startMidi();
}

AudioEffectFaust::~AudioEffectFaust()
{
    m_effectInstances.clear();
    m_midiHandler->stopMidi();
}

Ref<AudioEffectInstance> AudioEffectFaust::_instantiate()
{
    Ref<AudioEffectFaustInstance> ins;
    ins.instantiate();
    ins->m_base = Ref<AudioEffectFaust>(this);

    //Save an iterator to the list in the effect instance so we can erase it in the destructor
    m_effectInstances.push_back(ins.ptr());
    auto it = m_effectInstances.end();
    std::advance(it, -1);

    ins->m_listIter = it;
    ins->set_faust_dsp(m_faustScript);

    return ins;
}

void AudioEffectFaust::AddProperty(PropertyInfo const& property)
{
    m_propertyList.push_back(property);
}

void AudioEffectFaust::SetProperty(StringName const& name, Variant const& value)
{
    _set(name, value);
}

std::optional<Variant> AudioEffectFaust::GetProperty(StringName const& name)
{
    Variant out;
    return _get(name, out) ? out : std::optional<Variant>();
}

List<PropertyInfo>& AudioEffectFaust::GetPropertyList()
{
    return m_propertyList;
}

void AudioEffectFaust::NotifyPropertyChanged()
{
    notify_property_list_changed();
}

void AudioEffectFaust::set_faust_dsp(Ref<FaustScript> const& script)
{
    m_faustScript = script;
    for (auto effectInstance : m_effectInstances)
        effectInstance->set_faust_dsp(script);
}

void AudioEffectFaust::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_faust_dsp"), &AudioEffectFaust::get_faust_dsp);
    ClassDB::bind_method(D_METHOD("set_faust_dsp", "p_dsp"), &AudioEffectFaust::set_faust_dsp);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "dsp", PROPERTY_HINT_RESOURCE_TYPE, "FaustScript"), "set_faust_dsp", "get_faust_dsp");
}

bool AudioEffectFaust::_set(const StringName &p_path, const Variant &p_value)
{
    if (!m_dspUI) return false;

    std::string const path = String(p_path).utf8().get_data();

    if (m_dspUI->getFullpathMap().contains(path)   ||
        m_dspUI->getShortnameMap().contains(path)  ||
        m_dspUI->getLabelMap().contains(path))
    {
        m_dspUI->setParamValue(String(p_path).utf8().get_data(), p_value);
        return true;
    }

    return false;
}

bool AudioEffectFaust::_get(const StringName &p_path, Variant &r_ret) const
{
    if (!m_dspUI) return false;

    std::string const path = String(p_path).utf8().get_data();

    if (m_dspUI->getFullpathMap().contains(path)   ||
        m_dspUI->getShortnameMap().contains(path)  ||
        m_dspUI->getLabelMap().contains(path))
    {
        r_ret = m_dspUI->getParamValue(path);
        return true;
    }

    return false;
}

void AudioEffectFaust::_get_property_list(List<PropertyInfo> *p_list) const
{
    *p_list = m_propertyList;
}

//====== CALL FAUST PROCESS =======

AudioEffectFaustInstance::AudioEffectFaustInstance()
{
    m_input[0]  = new float[1024] {};
    m_input[1]  = new float[1024] {};

    m_output[0] = new float[1024] {};
    m_output[1] = new float[1024] {};
}

AudioEffectFaustInstance::~AudioEffectFaustInstance()
{
    delete[] m_input[0];
    delete[] m_input[1];

    delete[] m_output[0];
    delete[] m_output[1];

    delete m_pDspInstance;
    m_faustScript->Detach(m_faustScriptIt);
    m_base->m_effectInstances.erase(m_listIter);
}

void AudioEffectFaustInstance::_process(void const* pSrcFrames, AudioFrame* pDstFrames, int32 frameCount)
{
    if (m_pDspInstance == nullptr) return;

    AudioFrame* src = (AudioFrame*)pSrcFrames;

    for (int i = 0; i < frameCount; i++)
    {
         m_input[0][i] = src[i].left;
         m_input[1][i] = src[i].right;
    }

     m_pDspInstance->compute(frameCount, m_input, m_output);

    for (int i = 0; i < frameCount; i++)
    {
        pDstFrames[i].left  = m_output[0][i];
        pDstFrames[i].right = m_output[1][i];
    }
}

bool AudioEffectFaustInstance::_process_silence() const
{
    return false;
}

void AudioEffectFaustInstance::_bind_methods() {}

void AudioEffectFaustInstance::set_faust_dsp(Ref<FaustScript> const &script)
{
    if (!m_faustScript.is_null())
        m_faustScript->Detach(m_faustScriptIt);

    if (!script.is_valid() || script.is_null())
    {
        m_faustScript = script;
        m_pDspInstance = nullptr;
        m_base->emit_changed();
        return;
    }

    m_faustScript = script;
    m_faustScriptIt = script->Attach(*this);
    UpdateDsp();
}

void AudioEffectFaustInstance::UpdateDsp()
{
    std::vector<std::pair<StringName, Variant>> valueMap;

    //Save old dsp parameters value
    for (PropertyInfo& info : m_base->m_propertyList)
    {
        if (Variant out; m_base->_get(info.name, out))
            valueMap.emplace_back(info.name, out);
    }

    m_base->m_propertyList.clear();

    if (m_faustScript->get_dsp_factory() == nullptr) return;

    delete m_pDspInstance;
    m_base->m_dspUI.reset();

    m_pDspInstance = m_faustScript->get_dsp_factory()->createDSPInstance();

    m_base->m_dspUI = std::make_unique<GodotMapUI>(*m_base.ptr());
    m_pDspInstance->buildUserInterface(m_base->m_dspUI.get());
    m_pDspInstance->buildUserInterface(m_base->m_midiUI.get());
    m_pDspInstance->init(static_cast<int>(AudioServer::get_singleton()->get_mix_rate()));

    //restore parameters value
    for ( auto& [name, variant]: valueMap)
    {
        if (Variant out;m_base->_get(name, out))
            m_base->_set(name, variant);
    }

    m_base->NotifyPropertyChanged();
}
