#include "script/FaustScriptResourceFormatLoader.h"
#include "script/FaustScript.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

Ref<FaustScriptResourceFormatLoader> FaustScriptResourceFormatLoader::m_sLoader {};

PackedStringArray FaustScriptResourceFormatLoader::_get_recognized_extensions() const
{
    return PackedStringArray{ "dsp" };
}

bool FaustScriptResourceFormatLoader::_handles_type(const StringName &p_type) const
{
    return p_type == Script::get_class_static() || p_type == FaustScript::get_class_static();
}

String FaustScriptResourceFormatLoader::_get_resource_type(const String &p_path) const
{
    return p_path.get_extension() == "dsp" ? FaustScript::get_class_static() : "";
}

bool FaustScriptResourceFormatLoader::_exists(const String &p_path) const
{
    return FileAccess::file_exists(p_path);
}

Variant FaustScriptResourceFormatLoader::_load(const String &p_path, const String &p_original_path,
    bool p_use_sub_threads, int32_t p_cache_mode) const
{
    Ref<FaustScript> script;
    script.instantiate();

    switch (p_cache_mode) {
        case CACHE_MODE_IGNORE:
        case CACHE_MODE_IGNORE_DEEP:
            break;

        case CACHE_MODE_REUSE: {
            Ref<FaustScript> existing_script = ResourceLoader::get_singleton()->get_cached_ref(p_path);
            if (existing_script.is_null())
            {
                script->set_path(p_original_path);
                break;
            }

            script = existing_script;
            break;
        }

        case CACHE_MODE_REPLACE:
        case CACHE_MODE_REPLACE_DEEP:
            script->take_over_path(p_original_path);
            break;
    }

    script->set_source_code(FileAccess::get_file_as_string(p_path));
    Error status = script->reload();
    if (status == OK) return script;

    return status;
}

void FaustScriptResourceFormatLoader::Register()
{
    m_sLoader.instantiate();
    ResourceLoader::get_singleton()->add_resource_format_loader(m_sLoader);
}

void FaustScriptResourceFormatLoader::Unregister()
{
    ResourceLoader::get_singleton()->remove_resource_format_loader(m_sLoader);
    m_sLoader.unref();
}

void FaustScriptResourceFormatLoader::_bind_methods() {}
