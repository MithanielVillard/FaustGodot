#include "script/FaustEditorSyntaxHighLighter.h"

#include "godot_cpp/classes/code_highlighter.hpp"
#include "godot_cpp/classes/editor_interface.hpp"
#include "godot_cpp/classes/gd_script_syntax_highlighter.hpp"
#include "godot_cpp/classes/script_editor.hpp"
#include "godot_cpp/classes/text_edit.hpp"

using namespace godot;

Ref<FaustEditorSyntaxHighlighter> FaustEditorSyntaxHighlighter::m_sHighlighter {};


Ref<EditorSyntaxHighlighter> FaustEditorSyntaxHighlighter::_create() const
{
    Ref<FaustEditorSyntaxHighlighter> instance;

    instance.instantiate();
    instance->m_gdSyntaxHighlighter.instantiate();
    instance->m_pTextEdit = memnew(TextEdit);
    instance->m_pTextEdit->set_syntax_highlighter(instance->m_gdSyntaxHighlighter);

    return instance;
}

String FaustEditorSyntaxHighlighter::_get_name() const
{
    return "Faust";
}

PackedStringArray FaustEditorSyntaxHighlighter::_get_supported_languages() const
{
    PackedStringArray languages;
    languages.push_back("Faust");
    return languages;
}

Dictionary FaustEditorSyntaxHighlighter::_get_line_syntax_highlighting(int32_t p_line) const
{
    m_pTextEdit->set_text(get_text_edit()->get_text());
    return m_gdSyntaxHighlighter->get_line_syntax_highlighting(p_line);

    Dictionary dic;

    Dictionary red;
    red["color"] = Color {1.0f, 0.0f, 0.0f};
    Dictionary green;
    green["color"] = Color {0.0f, 1.0f, 0.0f};


    dic[0] = red;
    dic[5] = green;
    return dic;
}

void FaustEditorSyntaxHighlighter::_update_cache()
{
    m_gdSyntaxHighlighter->update_cache();
}

void FaustEditorSyntaxHighlighter::Register()
{
    m_sHighlighter.instantiate();
    EditorInterface::get_singleton()->get_script_editor()->register_syntax_highlighter(m_sHighlighter);
}

void FaustEditorSyntaxHighlighter::Unregister()
{
    EditorInterface::get_singleton()->get_script_editor()->unregister_syntax_highlighter(m_sHighlighter);
    m_sHighlighter.unref();
}

void FaustEditorSyntaxHighlighter::_bind_methods() {}
