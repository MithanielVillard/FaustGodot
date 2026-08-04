#include "inspector/VSliderEditorProperty.h"
#include <godot_cpp/classes/v_slider.hpp>

void godot::VSliderEditorProperty::setup(float min, float max, float step)
{
    m_pPropertyControl = memnew(VSlider);
    m_pPropertyControl->set_min(min);
    m_pPropertyControl->set_max(max);
    m_pPropertyControl->set_step(step);

    add_child(m_pPropertyControl);
    add_focusable(m_pPropertyControl);
    RefreshSliderValue();

    m_pPropertyControl->connect("value_changed", Callable(this, "on_value_change"));
}

void godot::VSliderEditorProperty::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("on_value_change"), &VSliderEditorProperty::OnValueChange);
}

void godot::VSliderEditorProperty::_update_property()
{
    double const newValue = get_edited_object()->get(get_edited_property());

    m_updating = true;
    m_currentValue = newValue;
    RefreshSliderValue();
    m_updating = false;
}

void godot::VSliderEditorProperty::RefreshSliderValue() const
{
    m_pPropertyControl->set_value(m_currentValue);
}

void godot::VSliderEditorProperty::OnValueChange(float value)
{
    // Ignore the signal if the property is currently being updated.
    if (m_updating) return;

    m_currentValue = value;
    RefreshSliderValue();
    emit_changed(get_edited_property(), m_currentValue);
}
