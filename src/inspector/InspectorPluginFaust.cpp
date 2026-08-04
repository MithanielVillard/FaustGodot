#include "inspector/InspectorPluginFaust.h"

#include "inspector/ButtonEditorProperty.h"
#include "inspector/VSliderEditorProperty.h"

using namespace godot;

bool InspectorPluginFaust::_can_handle(Object *p_object) const
{
    //Inspector plugin only handle Faust Godot object
     return (p_object->is_class("AudioEffectFaust") ||
        p_object->is_class("AudioStreamFaust"));
}

bool InspectorPluginFaust::_parse_property(Object *p_object,
    Variant::Type p_type,
    const String &p_name,
    PropertyHint p_hint_type,
    const String &p_hint_string,
    BitField<PropertyUsageFlags> p_usage_flags,
    bool p_wide)
{
    PackedStringArray const arguments = p_hint_string.split(",");

    if (p_hint_type == PropertyHint::PROPERTY_HINT_RANGE &&
        arguments.has("vertical"))
    {
        VSliderEditorProperty* editor = memnew(VSliderEditorProperty);
        editor->setup(arguments[0].to_float(), arguments[1].to_float(), arguments[2].to_float());
        add_property_editor(p_name, editor);
        return true;
    }

    if (p_hint_type == PropertyHint::PROPERTY_HINT_TOOL_BUTTON)
    {
        ButtonEditorProperty* editor = memnew(ButtonEditorProperty);
        editor->setup();
        add_property_editor(p_name, editor);
        return true;
    }

    return false;
}