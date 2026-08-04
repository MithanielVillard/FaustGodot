#pragma once

#include "defines.h"
#include "GodotDsp.h"
#include "GodotMidi.h"
#include "IPropertyHandler.h"

#include <godot_cpp/classes/audio_effect_instance.hpp>
#include <godot_cpp/classes/audio_effect.hpp>
#include <faust/gui/MidiUI.h>


class GodotDsp;
class GodotMapUI;

namespace godot
{
    class AudioEffectFaust;

    class AudioEffectFaustInstance : public AudioEffectInstance
    {
    GDCLASS(AudioEffectFaustInstance, AudioEffectInstance)

    public:
        AudioEffectFaustInstance();
        ~AudioEffectFaustInstance() override;

        void _process(void const* pSrcFrames, AudioFrame* pDstFrames, int32 frameCount) override;

        bool _process_silence() const override;

    protected:
        static void _bind_methods();

    private:
        friend AudioEffectFaust;

        GodotDsp m_dsp;
        Ref<AudioEffectFaust> m_base;

        //Hardcoded 2 inputs and outputs. Seems to be a godot hard limitation
        float* m_input[2] {};
        float* m_output[2] {};
    };


    class AudioEffectFaust : public AudioEffect, public IPropertyHandler
    {
    GDCLASS(AudioEffectFaust, AudioEffect)

    public:
        AudioEffectFaust();
        ~AudioEffectFaust() override;

        Ref<AudioEffectInstance> _instantiate() override;

        void AddProperty(PropertyInfo const& property) override;
        void SetProperty(StringName const& name, Variant const& value) override;
        std::optional<Variant> GetProperty(StringName const& name) override;
        List<PropertyInfo>& GetPropertyList() override;
        void NotifyPropertyChanged() override;

    protected:
        static void _bind_methods();
       	bool _set(const StringName &p_path, const Variant &p_value);
        bool _get(const StringName &p_path, Variant &r_ret) const;
        void _get_property_list(List<PropertyInfo> *p_list) const;

       	friend class AudioEffectFaustInstance;
    private:
        uptr<GodotMidi> m_midiHandler;
        uptr<MidiUI> m_midiUI;
        uptr<GodotMapUI> m_dspUI;

        List<PropertyInfo> m_propertyList;
    };

}
