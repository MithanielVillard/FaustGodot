#include "gdexample.h"
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

struct FaustBinder
{
    static void Bind()
    {
        ClassDB::bind_method(D_METHOD("get_faust_dsp"), &AudioStreamFaust::get_faust_dsp);
        ClassDB::bind_method(D_METHOD("set_faust_dsp", "p_dsp"), &AudioStreamFaust::set_faust_dsp);
    }
};

// void AudioEffectFaust::_bind_methods() {}
void AudioStreamFaust::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_faust_dsp"), &AudioStreamFaust::get_faust_dsp);
    ClassDB::bind_method(D_METHOD("set_faust_dsp", "p_dsp"), &AudioStreamFaust::set_faust_dsp);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "dsp", PROPERTY_HINT_RESOURCE_TYPE), "set_faust_dsp", "get_faust_dsp");
}

bool InspectorPlugin::_can_handle(Object *p_object) const
{
    return false;
}

bool InspectorPlugin::_parse_property(Object *p_object,
    Variant::Type p_type,
    const String &p_name,
    PropertyHint p_hint_type,
    const String &p_hint_string,
    BitField<PropertyUsageFlags> p_usage_flags,
    bool p_wide)
{
    if (p_type != Variant::Type::INT) return false;

    UtilityFunctions::print("parse property called");
    RandomIntEditor* editor = memnew(RandomIntEditor);
    editor->setup();
    add_property_editor(p_name, editor);
    return true;
}
