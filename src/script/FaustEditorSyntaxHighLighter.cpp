#include "script/FaustEditorSyntaxHighLighter.h"

#include <string_view>
#include <godot_cpp/classes/code_highlighter.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/script_editor.hpp>

using namespace godot;

Ref<FaustEditorSyntaxHighlighter> FaustEditorSyntaxHighlighter::m_sHighlighter {};

FaustEditorSyntaxHighlighter::~FaustEditorSyntaxHighlighter()
{
    if (m_pCodeEdit) m_pCodeEdit->queue_free();
}

Ref<EditorSyntaxHighlighter> FaustEditorSyntaxHighlighter::_create() const
{
    Ref<FaustEditorSyntaxHighlighter> instance;

    instance.instantiate();
    instance->m_codeHighlighter.instantiate();
    instance->m_pCodeEdit = memnew(CodeEdit);
    instance->m_pCodeEdit->set_language("Faust");
    instance->m_pCodeEdit->set_syntax_highlighter(instance->m_codeHighlighter);\

    std::array keywords {
        "import", "component", "declare", "library", "environment", "int", "float",
        "letrec", "with", "class", "process", "effect", "inputs", "outputs"
    };

    Dictionary dic;
    for (const char* keyword : keywords)
        dic.set(keyword, Color::from_rgba8(68, 153, 204));

    instance->m_codeHighlighter->set_keyword_colors(dic);
    instance->m_codeHighlighter->set_function_color(Color::from_rgba8(221, 221, 153));
    instance->m_codeHighlighter->set_symbol_color(Color::from_rgba8(255, 221, 255));
    instance->m_codeHighlighter->set_member_variable_color(Color::from_rgba8(204, 204, 187));
    instance->m_codeHighlighter->set_number_color(Color::from_rgba8(181, 206, 168));
    instance->m_codeHighlighter->add_color_region("//", "", Color::from_rgba8(106, 153, 85));

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
    m_pCodeEdit->set_text(get_text_edit()->get_text());
    return m_codeHighlighter->get_line_syntax_highlighting(p_line);
}

void FaustEditorSyntaxHighlighter::_update_cache()
{
    m_codeHighlighter->update_cache();
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
