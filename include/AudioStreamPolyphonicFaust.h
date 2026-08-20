#pragma once
#include "AudioStreamFaust.h"

namespace godot
{
    class AudioStreamPolyphonicFaust : public AudioStreamFaust
    {
    GDCLASS(AudioStreamPolyphonicFaust, AudioStreamFaust)

    public:
        ~AudioStreamPolyphonicFaust() override;

        void set_nbr_voices(int32 nbr_voices);
        int32 get_nbr_voices() const { return m_nbrVoices; }

        void set_global_effect(Ref<FaustScript> const& global_effect);
        Ref<FaustScript> get_global_effect() const { return m_globalEffect; }

        void set_midi_control(bool midi);
        bool get_midi_control() const { return m_midiControl; }

        void UpdateDsp() override;

    protected:
        static void _bind_methods();

    private:
        int32 m_nbrVoices = 1;
        Ref<FaustScript> m_globalEffect;
        bool m_midiControl = false;
    };
}
