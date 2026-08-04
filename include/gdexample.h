#pragma once

#include <functional>
#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/v_slider.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/templates/list.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
    // class AudioEffectFaust : public AudioEffect
    // {
    // GDCLASS(AudioEffectFaust, AudioEffect)
    //
    // public:
    //     AudioEffectFaust() = default;
    //     ~AudioEffectFaust() = default;
    //
    // protected:
    //     static void _bind_methods();
    // };



    class AudioStreamFaust : public AudioStream
    {
    GDCLASS(AudioStreamFaust, AudioStream)

    public:
        AudioStreamFaust() = default;
        ~AudioStreamFaust() = default;

        void set_faust_dsp(Ref<Resource> p_dsp) { m_faustDSP = p_dsp; }
        Ref<Resource> get_faust_dsp() const { return m_faustDSP; }

    protected:
        static void _bind_methods();

    private:
        Ref<Resource> m_faustDSP;
    };



    class ExtButton : public VSlider
    {
    GDCLASS(ExtButton, VSlider)

    public:
        std::function<void()> OnPressed;
        //void _pressed() override { OnPressed(); }

    protected:
        static void _bind_methods() {};
    };


    class RandomIntEditor : public EditorProperty
    {
        GDCLASS(RandomIntEditor, EditorProperty)

    public:
        void setup()
        {
            m_pPropertyControl = memnew(ExtButton);

            add_child(m_pPropertyControl);
            add_focusable(m_pPropertyControl);
            RefreshControlText();

            m_pPropertyControl->OnPressed = [&](){ OnButtonPressed(); };
        }

        void _update_property() override
        {
            int newValue = get_edited_object()->get(get_edited_property());
            if (newValue == m_currentValue) return;

            m_updating = true;
            m_currentValue = newValue;
            RefreshControlText();
            m_updating = false;
        }

    protected:
        static void _bind_methods() {};

    private:
        void OnButtonPressed()
        {
            if (m_updating) return;

            m_currentValue += 10;
            RefreshControlText();
            emit_changed(get_edited_property(), m_currentValue);
        }

        void RefreshControlText() {
            m_pPropertyControl->set_value(
                m_currentValue
            );
        }

    private:
        ExtButton* m_pPropertyControl {nullptr};
        int m_currentValue;
        bool m_updating = false;
    };


    class InspectorPlugin : public EditorInspectorPlugin
    {
        GDCLASS(InspectorPlugin, EditorInspectorPlugin)

    public:
        bool _can_handle(Object *p_object) const override;
        bool _parse_property(Object *p_object,
            Variant::Type p_type,
            const String &p_name,
            PropertyHint p_hint_type,
            const String &p_hint_string,
            BitField<PropertyUsageFlags> p_usage_flags,
            bool p_wide) override;

    protected:
        static void _bind_methods() {};
    };


    class Plugin : public EditorPlugin
    {
    GDCLASS(Plugin, EditorPlugin)

    public:
        void _enter_tree() override
        {
            m_myInspectorPlugin.instantiate();
            add_inspector_plugin(m_myInspectorPlugin);
            UtilityFunctions::print("Loaded editor plugin");
        }
        void _exit_tree() override
        {
            remove_inspector_plugin(m_myInspectorPlugin);
            m_myInspectorPlugin.unref();
        }

    protected:
        static void _bind_methods() {};

        Ref<InspectorPlugin> m_myInspectorPlugin;
    };
}
