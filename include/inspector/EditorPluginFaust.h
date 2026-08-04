#pragma once
#include "inspector/InspectorPluginFaust.h"

#include <godot_cpp/classes/editor_plugin.hpp>

namespace godot
{

    class EditorPluginFaust : public EditorPlugin
    {
    GDCLASS(EditorPluginFaust, EditorPlugin)

    public:
    void _enter_tree() override;
    void _exit_tree() override;

    protected:
        static void _bind_methods() {};

    protected:
        Ref<InspectorPluginFaust> m_inspectorPlugin;
    };

}
