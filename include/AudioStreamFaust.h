#pragma once

#include "defines.h"
#include "GodotMapUI.h"
#include "GodotMidi.h"
#include "IPropertyHandler.h"
#include "script/FaustScript.h"
#include "IFaustHandler.h"

#include <faust/gui/MidiUI.h>
#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_playback.hpp>

namespace godot
{
    class AudioStreamFaust;

    class AudioStreamPlaybackFaust : public AudioStreamPlayback
    {
    GDCLASS(AudioStreamPlaybackFaust, AudioStreamPlayback)

    public:
        AudioStreamPlaybackFaust();
        ~AudioStreamPlaybackFaust() override;

        void _start(double p_from_pos = 0.0) override { m_active = true; }
        void _stop() override { m_active = false; }
        bool _is_playing() const override { return m_active; }

        int32_t _mix(AudioFrame* p_buffer, float p_rate_scale, int32_t p_frames) override;

    protected:
        static void _bind_methods();

    private:
        static constexpr int32 m_bufferSize { 4096 }; //TODO CHANGE
        bool m_active { true };
        uint32 m_pos {};

        Ref<AudioStreamFaust> m_base;

        float* m_output[2] {};

        friend class AudioStreamFaust;
    };


    class AudioStreamFaust : public AudioStream, public IFaustHandler, public IPropertyHandler
    {
    GDCLASS(AudioStreamFaust, AudioStream)

    public:
        AudioStreamFaust();
        ~AudioStreamFaust() override;

        Ref<AudioStreamPlayback> _instantiate_playback() const override;
        String _get_stream_name() const override;
        double _get_length() const override { return 0; }

        void AddProperty(PropertyInfo const& property) override;
        void SetProperty(StringName const& name, Variant const& value) override;
        std::optional<Variant> GetProperty(StringName const& name) override;
        List<PropertyInfo>& GetPropertyList() override;
        void NotifyPropertyChanged() override;

        void set_faust_dsp(Ref<FaustScript> const& script) override;
        Ref<FaustScript> get_faust_dsp() const override { return m_faustScript; }

        void UpdateDsp() override;
    protected:
        static void _bind_methods();
        bool _set(const StringName &p_path, const Variant &p_value);
        bool _get(const StringName &p_path, Variant &r_ret) const;
        void _get_property_list(List<PropertyInfo> *p_list) const;

        friend class AudioStreamPlaybackFaust;

    private:
        uptr<GodotMapUI> m_dspUI;
        uptr<GodotMidi> m_midiHandler;
        uptr<MidiUI> m_midiUI;

        dsp* m_pDspInstance;

        FaustScript::ListIt m_faustScriptIt;
        Ref<FaustScript> m_faustScript;

        List<PropertyInfo> m_propertyList;
    };

}

