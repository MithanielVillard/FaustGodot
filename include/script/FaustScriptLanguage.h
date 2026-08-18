#pragma once

#include <godot_cpp/classes/script_language_extension.hpp>
#include <godot_cpp/classes/window.hpp>

namespace godot
{
    class FaustScriptLanguage : public ScriptLanguageExtension
    {
    GDCLASS(FaustScriptLanguage, ScriptLanguageExtension)

    public:
        void _init() override;
        void _finish() override;
        void _thread_enter() override;
        void _thread_exit() override;

        String _get_name() const override;
        String _get_type() const override;
        String _get_extension() const override;
        PackedStringArray _get_recognized_extensions() const override;
        bool _can_inherit_from_file() const override;
        bool _supports_builtin_mode() const override;
        bool _is_using_templates() override;
        bool _handles_global_class_type(const String &p_type) const override;
        String _validate_path(const String &p_path) const override;
        Ref<Script> _make_template(const String &p_template, const String &p_class_name, const String &p_base_class_name) const override;
        void _frame() override;
        bool _overrides_external_editor() override;
        Dictionary _validate(const String &p_script, const String &p_path, bool p_validate_functions, bool p_validate_errors, bool p_validate_warnings, bool p_validate_safe_lines) const override;
        void _reload_scripts(const Array& p_scripts, bool p_soft_reload) override;
        TypedArray<Dictionary> _debug_get_current_stack_info() override;

        bool _supports_documentation() const override;
        Dictionary _complete_code(const String &p_code, const String &p_path, Object *p_owner) const override;
        Dictionary _lookup_code(const String &p_code, const String &p_symbol, const String &p_path, Object *p_owner) const override;

        PackedStringArray _get_comment_delimiters() const override;
        PackedStringArray _get_string_delimiters() const override;
        bool _is_control_flow_keyword(const String &p_keyword) const override;
        PackedStringArray _get_reserved_words() const override;

        static FaustScriptLanguage* get_instance();
        static void destroy_instance();

        static void Register();
        static void Unregister();

    protected:
        static void _bind_methods();

    private:
        static FaustScriptLanguage* m_pInstance;

        Window* m_pPopup {};
    };
}
