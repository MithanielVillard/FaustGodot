#include "AudioStreamPolyphonicFaust.h"

#include <godot_cpp/classes/audio_server.hpp>
#include <faust/dsp/poly-dsp.h>

godot::AudioStreamPolyphonicFaust::~AudioStreamPolyphonicFaust()
{
    m_globalEffect.unref();
}

void godot::AudioStreamPolyphonicFaust::set_nbr_voices(int32 const nbr_voices)
{
    m_nbrVoices = nbr_voices;
    UpdateDsp();
}

void godot::AudioStreamPolyphonicFaust::set_global_effect(Ref<FaustScript> const &global_effect)
{
    m_globalEffect = global_effect;
    UpdateDsp();
}

void godot::AudioStreamPolyphonicFaust::set_midi_control(bool const midi)
{
    m_midiControl = midi;
    UpdateDsp();
}

void godot::AudioStreamPolyphonicFaust::UpdateDsp()
{
    if (m_faustScript.is_null() || !m_faustScript.is_valid()) return;

    std::vector<std::pair<StringName, Variant>> valueMap;

    //Save old dsp parameters value
    for (PropertyInfo& info : m_propertyList)
    {
        if (Variant out; _get(info.name, out))
            valueMap.emplace_back(info.name, out);
    }

    m_propertyList.clear();

    // was used previously instead of deleting then recreating but causing issues because the MapUI is not fully cleared
    // m_dspUI->getFullpathMap().clear();
    // m_dspUI->getLabelMap().clear();
    // m_dspUI->getShortnameMap().clear();

    if (m_faustScript->get_dsp_factory() == nullptr) return;

    delete m_pDspInstance;
    m_dspUI.reset();

    m_pDspInstance = new mydsp_poly(m_faustScript->get_dsp_factory()->createDSPInstance(), m_nbrVoices, m_midiControl, true);

    if (!m_globalEffect.is_null() && m_globalEffect.is_valid())
        m_pDspInstance = new dsp_sequencer(m_pDspInstance, m_globalEffect->get_dsp_factory()->createDSPInstance());

    m_dspUI = std::make_unique<GodotMapUI>(*this);
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

void godot::AudioStreamPolyphonicFaust::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_nbr_voices"), &AudioStreamPolyphonicFaust::get_nbr_voices);
    ClassDB::bind_method(D_METHOD("set_nbr_voices", "p_nbr_voices"), &AudioStreamPolyphonicFaust::set_nbr_voices);

    ClassDB::bind_method(D_METHOD("get_global_effect"), &AudioStreamPolyphonicFaust::get_global_effect);
    ClassDB::bind_method(D_METHOD("set_global_effect", "p_global_effect"), &AudioStreamPolyphonicFaust::set_global_effect);

    ClassDB::bind_method(D_METHOD("get_midi_control"), &AudioStreamPolyphonicFaust::get_midi_control);
    ClassDB::bind_method(D_METHOD("set_midi_control", "p_global_effect"), &AudioStreamPolyphonicFaust::set_midi_control);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "number_voices"), "set_nbr_voices", "get_nbr_voices");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "global_effect", PROPERTY_HINT_RESOURCE_TYPE, "FaustScript"), "set_global_effect", "get_global_effect");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "midi_control"), "set_midi_control", "get_midi_control");
}
