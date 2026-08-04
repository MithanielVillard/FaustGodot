#include "AudioStreamFaust.h"
#include "GodotMapUI.h"

#include <faust/gui/MidiUI.h>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

AudioStreamFaust::AudioStreamFaust()
{
    m_dspUI       = std::make_unique<GodotMapUI>(*this);
    m_midiHandler = std::make_unique<GodotMidi>();
    m_midiUI      = std::make_unique<MidiUI>(m_midiHandler.get());

    m_midiHandler->startMidi();
}

AudioStreamFaust::~AudioStreamFaust()
{
    m_midiHandler->stopMidi();
    if (!m_faustScript.is_null())
        m_faustScript->Detach(m_faustScriptIt);
}

Ref<AudioStreamPlayback> AudioStreamFaust::_instantiate_playback() const
{
    Ref<AudioStreamPlaybackFaust> instance;
    instance.instantiate();
    instance->m_base = Ref<AudioStreamFaust>(this);

    return instance;
}

void AudioStreamFaust::AddProperty(PropertyInfo const& property)
{
    m_propertyList.push_back(property);
}

void AudioStreamFaust::SetProperty(StringName const& name, Variant const& value)
{
    _set(name, value);
}

std::optional<Variant> AudioStreamFaust::GetProperty(StringName const& name)
{
    Variant out;
    return _get(name, out) ? out : std::optional<Variant>();
}

List<PropertyInfo>& AudioStreamFaust::GetPropertyList()
{
    return m_propertyList;
}

void AudioStreamFaust::NotifyPropertyChanged()
{
    notify_property_list_changed();
}

void AudioStreamFaust::set_faust_dsp(Ref<FaustScript> const &script)
{
    if (!m_faustScript.is_null())
        m_faustScript->Detach(m_faustScriptIt);

    if (!script.is_valid() || script.is_null())
    {
        m_faustScript = script;
        m_pDspInstance = nullptr;
        emit_changed();
        return;
    }

    m_faustScript = script;
    m_faustScriptIt = script->Attach(*this);
    UpdateDsp();
}

String AudioStreamFaust::_get_stream_name() const
{
    return "AudioStreamFaust";
}

void AudioStreamFaust::UpdateDsp()
{
    std::vector<std::pair<StringName, Variant>> valueMap;

    //Save old dsp parameters value
    for (PropertyInfo& info : m_propertyList)
    {
        if (Variant out; _get(info.name, out))
            valueMap.emplace_back(info.name, out);
    }

    m_propertyList.clear();
    m_dspUI->getFullpathMap().clear();
    m_dspUI->getLabelMap().clear();
    m_dspUI->getShortnameMap().clear();

    m_pDspInstance = m_faustScript->get_dsp_factory()->createDSPInstance();
    m_pDspInstance->buildUserInterface(m_dspUI.get());
    m_pDspInstance->buildUserInterface(m_midiUI.get());
    m_pDspInstance->init(static_cast<int>(AudioServer::get_singleton()->get_mix_rate()));

    //restore parameters value
    for ( auto& [name, variant]: valueMap)
    {
        if (Variant out; _get(name, out))
            _set(name, variant);
    }

    notify_property_list_changed();
}

void AudioStreamFaust::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_faust_dsp"), &AudioStreamFaust::get_faust_dsp);
    ClassDB::bind_method(D_METHOD("set_faust_dsp", "p_dsp"), &AudioStreamFaust::set_faust_dsp);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "dsp", PROPERTY_HINT_RESOURCE_TYPE, "FaustScript"), "set_faust_dsp", "get_faust_dsp");
}

bool AudioStreamFaust::_set(const StringName &p_path, const Variant &p_value)
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

bool AudioStreamFaust::_get(const StringName &p_path, Variant &r_ret) const
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

void AudioStreamFaust::_get_property_list(List<PropertyInfo> *p_list) const
{
    *p_list = m_propertyList;
}

AudioStreamPlaybackFaust::AudioStreamPlaybackFaust()
{
    m_output[0] = new float[m_bufferSize] {};
    m_output[1] = new float[m_bufferSize] {};
}

AudioStreamPlaybackFaust::~AudioStreamPlaybackFaust()
{
    delete[] m_output[0];
    delete[] m_output[1];
}

int32_t AudioStreamPlaybackFaust::_mix(AudioFrame* p_buffer, float p_rate_scale, int32_t p_frames)
{
    if (!m_active || m_base->m_pDspInstance == nullptr) return 0;

    m_base->m_pDspInstance->compute(p_frames, nullptr, m_output);

    //Deinterlace
    for (int i = 0; i < p_frames; i++)
    {
        p_buffer[i].left  = m_output[0][i];
        p_buffer[i].right = m_output[1][i];
    }

    return p_frames;
}

void AudioStreamPlaybackFaust::_bind_methods(){}
