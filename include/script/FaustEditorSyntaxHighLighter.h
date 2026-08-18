#pragma once


#include <godot_cpp/classes/editor_syntax_highlighter.hpp>
#include <godot_cpp/classes/text_edit.hpp>

#include "godot_cpp/classes/code_edit.hpp"
#include "godot_cpp/classes/code_highlighter.hpp"

namespace godot
{

class FaustEditorSyntaxHighlighter : public EditorSyntaxHighlighter
{
GDCLASS(FaustEditorSyntaxHighlighter, EditorSyntaxHighlighter)

public:
    ~FaustEditorSyntaxHighlighter();

    Ref<EditorSyntaxHighlighter> _create() const override;
    String _get_name() const override;
    PackedStringArray _get_supported_languages() const override;

    Dictionary _get_line_syntax_highlighting(int32_t p_line) const override;
    void _update_cache() override;

    static void Register();
    static void Unregister();

protected:
    static void _bind_methods();

private:
    static Ref<FaustEditorSyntaxHighlighter> m_sHighlighter;
    Ref<CodeHighlighter> m_codeHighlighter;
    CodeEdit* m_pCodeEdit {};
    Ref<CodeEdit> m_codeEdit;
};

}

