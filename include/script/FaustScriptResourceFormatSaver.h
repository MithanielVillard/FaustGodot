#pragma once

#include <godot_cpp/classes/resource_format_saver.hpp>
#include <godot_cpp/classes/resource.hpp>

namespace godot
{
    class FaustScriptResourceFormatSaver : public ResourceFormatSaver
    {
    GDCLASS(FaustScriptResourceFormatSaver, ResourceFormatSaver)

    public:
        Error _save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags) override;
        bool _recognize(const Ref<Resource> &p_resource) const override;
        PackedStringArray _get_recognized_extensions(const Ref<Resource> &p_resource) const override;

        static void Register();
        static void Unregister();

    protected:
        static void _bind_methods();

    private:
        static Ref<FaustScriptResourceFormatSaver> m_sFormatter;
    };
}
