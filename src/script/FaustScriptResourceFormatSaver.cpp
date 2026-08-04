#include "script/FaustScriptResourceFormatSaver.h"
#include "script/FaustScript.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_saver.hpp>

using namespace godot;

Ref<FaustScriptResourceFormatSaver> FaustScriptResourceFormatSaver::m_sFormatter {};

Error FaustScriptResourceFormatSaver::_save(const Ref<Resource> &p_resource, const String &p_path,
                                                          uint32_t p_flags)
{
    Ref<FaustScript> const lua_script = p_resource;
    ERR_FAIL_COND_V(lua_script.is_null(), ERR_INVALID_PARAMETER);

    Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::ModeFlags::WRITE);
    ERR_FAIL_COND_V_MSG(file.is_null(), FileAccess::get_open_error(), "Failed to save file at " + p_path);

    file->store_string(lua_script->get_source_code());
    switch (Error const error = file->get_error()) {
        default:
            return error;
        case OK:
        case ERR_FILE_EOF:
            return OK;
    }
}

bool FaustScriptResourceFormatSaver::_recognize(const Ref<Resource> &p_resource) const
{
    Ref<FaustScript> const script = p_resource;
    return script.is_valid();
}

PackedStringArray FaustScriptResourceFormatSaver::_get_recognized_extensions(const Ref<Resource> &p_resource) const
{
    return PackedStringArray{_recognize(p_resource) ? "dsp" : ""};
}

void FaustScriptResourceFormatSaver::Register()
{
    m_sFormatter.instantiate();
    ResourceSaver::get_singleton()->add_resource_format_saver(m_sFormatter);
}

void FaustScriptResourceFormatSaver::Unregister()
{
    ResourceSaver::get_singleton()->remove_resource_format_saver(m_sFormatter);
    m_sFormatter.unref();
}

void FaustScriptResourceFormatSaver::_bind_methods() {}
