#include "inspector/EditorPluginFaust.h"
#include "script/FaustEditorSyntaxHighLighter.h"

#include <godot_cpp/classes/editor_interface.hpp>

void godot::EditorPluginFaust::_enter_tree()
{
    m_inspectorPlugin.instantiate();
    add_inspector_plugin(m_inspectorPlugin);

    FaustEditorSyntaxHighlighter::Register();
}

void godot::EditorPluginFaust::_exit_tree()
{
    remove_inspector_plugin(m_inspectorPlugin);
    m_inspectorPlugin.unref();

    FaustEditorSyntaxHighlighter::Unregister();
}
