#include "script/FaustScriptLanguage.h"
#include "script/FaustScript.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

FaustScriptLanguage* FaustScriptLanguage::m_pInstance {};

void FaustScriptLanguage::_init()
{
    ScriptLanguageExtension::_init();
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

bool FaustScriptLanguage::_supports_documentation() const
{
    return true;
}

Dictionary FaustScriptLanguage::_complete_code(const String &p_code, const String &p_path, Object *p_owner) const
{
    Dictionary dic {};
    dic.set("call_hint", "Call hint");
    dic.set("force", "true");
    return dic;
}

Dictionary FaustScriptLanguage::_lookup_code(const String &p_code, const String &p_symbol, const String &p_path,
                                             Object *p_owner) const
{
    UtilityFunctions::print(p_symbol);
    Dictionary dic {};
    dic.set("result", OK);
    dic.set("type", LOOKUP_RESULT_CLASS_METHOD);
    dic.set("class_name", "FaustScript");
    dic.set("class_member", "osc");
    dic.set("description", "osc");
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
