#include "script/FaustScriptLanguage.h"
#include "script/FaustScript.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/editor_interface.hpp>

using namespace godot;

FaustScriptLanguage* FaustScriptLanguage::m_pInstance {};

void FaustScriptLanguage::_init()
{
    ScriptLanguageExtension::_init();
}

void FaustScriptLanguage::_finish()
{
    //m_pPopup->queue_free();
    ScriptLanguageExtension::_finish();
}

void FaustScriptLanguage::_thread_enter()
{
    ScriptLanguageExtension::_thread_enter();
}

void FaustScriptLanguage::_thread_exit()
{
    ScriptLanguageExtension::_thread_exit();
}

String FaustScriptLanguage::_get_name() const
{
    return "Faust";
}

String FaustScriptLanguage::_get_type() const
{
    return "FaustScript";
}

String FaustScriptLanguage::_get_extension() const
{
    return "dsp";
}

PackedStringArray FaustScriptLanguage::_get_recognized_extensions() const
{
    return PackedStringArray {"dsp" };
}

bool FaustScriptLanguage::_can_inherit_from_file() const
{
    return false;
}

bool FaustScriptLanguage::_supports_builtin_mode() const
{
    return false;
}

bool FaustScriptLanguage::_is_using_templates()
{
    return false;
}

bool FaustScriptLanguage::_handles_global_class_type(const String &p_type) const
{
    return false;
}

String FaustScriptLanguage::_validate_path(const String &p_path) const
{
    return ScriptLanguageExtension::_validate_path(p_path);
}

Ref<Script> FaustScriptLanguage::_make_template(const String &p_template, const String &p_class_name,
    const String &p_base_class_name) const
{
    // return empty script
    Ref<FaustScript> script = memnew(FaustScript);
    return script;
}

void FaustScriptLanguage::_frame()
{
    ScriptLanguageExtension::_frame();
}

bool FaustScriptLanguage::_overrides_external_editor()
{
    return false;
}

Dictionary FaustScriptLanguage::_validate(const String &p_script, const String &p_path, bool p_validate_functions,
    bool p_validate_errors, bool p_validate_warnings, bool p_validate_safe_lines) const
{
    return ScriptLanguageExtension::_validate(p_script, p_path, p_validate_functions, p_validate_errors,
                                              p_validate_warnings,
                                              p_validate_safe_lines);
}

void FaustScriptLanguage::_reload_scripts(const Array& p_scripts, bool p_soft_reload)
{
    ScriptLanguageExtension::_reload_scripts(p_scripts, p_soft_reload);
}

TypedArray<Dictionary> FaustScriptLanguage::_debug_get_current_stack_info()
{
    return ScriptLanguageExtension::_debug_get_current_stack_info();
}

bool FaustScriptLanguage::_supports_documentation() const
{
    return false;
}

Dictionary FaustScriptLanguage::_complete_code(const String &p_code, const String &p_path, Object *p_owner) const
{
    //TODO ADD CODE COMPLETION
    return {};
}

Dictionary FaustScriptLanguage::_lookup_code(const String &p_code, const String &p_symbol, const String &p_path,
                                             Object *p_owner) const
{
    //TODO ADD MOUSE CODE DOCUMENTATION LOOKUP

    //this was a try at making a custom popup for documentation
    // m_pInstance->m_pPopup = memnew(Window);
    // m_pInstance->m_pPopup->set_initial_position(Window::WINDOW_INITIAL_POSITION_ABSOLUTE);
    // m_pInstance->m_pPopup->set_size(Vector2i(200, 100));
    // m_pInstance->m_pPopup->set_flag(Window::FLAG_BORDERLESS, true);
    // m_pInstance->m_pPopup->set_flag(Window::FLAG_ALWAYS_ON_TOP, true);
    // m_pInstance->m_pPopup->set_flag(Window::FLAG_NO_FOCUS, true);
    // m_pInstance->m_pPopup->set_flag(Window::FLAG_POPUP, true);

    //m_pPopup->set_position(EditorInterface::get_singleton()->get_editor_main_screen()->get_global_mouse_position());
    //EditorInterface::get_singleton()->popup_dialog(m_pPopup);

    Dictionary dic;
    dic["result"] = LOOKUP_RESULT_CLASS;
    dic["type"] = "";

    return dic;
}

PackedStringArray FaustScriptLanguage::_get_comment_delimiters() const
{
    return helpers::append_all(PackedStringArray(),
        "//", "/*[[ ]]*/"
    );
}

PackedStringArray FaustScriptLanguage::_get_string_delimiters() const
{
    return PackedStringArray{"\" \""};
}

bool FaustScriptLanguage::_is_control_flow_keyword(const String &p_keyword) const
{
    return helpers::append_all(PackedStringArray(),
        "<:", ":>", "_", ":", ",", "~"
    ).has(p_keyword);
}

PackedStringArray FaustScriptLanguage::_get_reserved_words() const
{
    return {"import", "component", "declare", "library", "environment", "int", "float",
    "letrec", "with", "class", "process", "effect", "inputs", "outputs"};
}

FaustScriptLanguage* FaustScriptLanguage::get_instance()
{
    if (!m_pInstance) m_pInstance = memnew(FaustScriptLanguage);
    return m_pInstance;
}

void FaustScriptLanguage::destroy_instance()
{
    if (m_pInstance)
    {
        memdelete(m_pInstance);
        m_pInstance = nullptr;
    }
}

void FaustScriptLanguage::Register()
{
    Engine::get_singleton()->register_script_language(get_instance());
}

void FaustScriptLanguage::Unregister()
{
    Engine::get_singleton()->unregister_script_language(get_instance());
    destroy_instance();
}

void FaustScriptLanguage::_bind_methods(){}
