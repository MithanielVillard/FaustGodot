#include "faust2godot.h"
#include "MidiHandlerFaust.h"
#include "AudioEffectFaust.h"
#include "AudioStreamFaust.h"

#include "script/FaustScript.h"
#include "script/FaustScriptLanguage.h"
#include "script/FaustScriptResourceFormatSaver.h"
#include "script/FaustEditorSyntaxHighLighter.h"
#include "script/FaustScriptResourceFormatLoader.h"

#include "inspector/VSliderEditorProperty.h"
#include "inspector/InspectorPluginFaust.h"
#include "inspector/EditorPluginFaust.h"
#include "inspector/ButtonEditorProperty.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm-c/Target.h>

using namespace godot;

void InitializeFaust2GodotModule(ModuleInitializationLevel level)
{
    if (level == MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        GDREGISTER_CLASS(MidiHandlerFaust);

        GDREGISTER_CLASS(AudioStreamPlaybackFaust);
        GDREGISTER_CLASS(AudioStreamFaust);
        GDREGISTER_CLASS(AudioEffectFaustInstance);
        GDREGISTER_CLASS(AudioEffectFaust);
    }

    if (level == MODULE_INITIALIZATION_LEVEL_EDITOR)
    {
        GDREGISTER_CLASS(VSliderEditorProperty);
        GDREGISTER_CLASS(ButtonEditorProperty)

        GDREGISTER_CLASS(InspectorPluginFaust);
        GDREGISTER_CLASS(EditorPluginFaust);

        GDREGISTER_CLASS(FaustScript)
        GDREGISTER_CLASS(FaustScriptLanguage)
        GDREGISTER_CLASS(FaustScriptResourceFormatSaver)
        GDREGISTER_CLASS(FaustScriptResourceFormatLoader)
        GDREGISTER_CLASS(FaustEditorSyntaxHighlighter)

        FaustScriptLanguage::Register();
        FaustScriptResourceFormatSaver::Register();
        FaustScriptResourceFormatLoader::Register();

        EditorPlugins::add_by_type<EditorPluginFaust>();

        llvm::InitializeNativeTarget();
    }
}

void UninitializeFaust2GodotModule(ModuleInitializationLevel level)
{
    if (level == MODULE_INITIALIZATION_LEVEL_EDITOR)
    {
        FaustScriptLanguage::Unregister();
        FaustScriptResourceFormatSaver::Unregister();
        FaustScriptResourceFormatLoader::Unregister();
    }
}

extern "C"
{
    //GDExtension entry point
    GDExtensionBool GDE_EXPORT faust2godot_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
        const GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization *r_initialization)
    {
    	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(InitializeFaust2GodotModule);
        init_obj.register_terminator(UninitializeFaust2GodotModule);
       	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);

        return init_obj.init();
    }
}
