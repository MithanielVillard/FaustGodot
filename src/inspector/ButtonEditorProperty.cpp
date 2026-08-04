#include "inspector/ButtonEditorProperty.h"
#include <godot_cpp/classes/button.hpp>

void godot::ButtonEditorProperty::setup()
{
    m_pPropertyControl = memnew(Button);

    add_child(m_pPropertyControl);
    add_focusable(m_pPropertyControl);
    RefreshButtonValue();

    m_pPropertyControl->connect("button_down", Callable(this, "on_button_down"));
    m_pPropertyControl->connect("button_up", Callable(this, "on_button_up"));

}

void godot::ButtonEditorProperty::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("on_button_down"), &ButtonEditorProperty::OnButtonDown);
    ClassDB::bind_method(D_METHOD("on_button_up"), &ButtonEditorProperty::OnButtonUp);
}

void godot::ButtonEditorProperty::_update_property()
{
    bool const newValue = get_edited_object()->get(get_edited_property());

    m_updating = true;
    m_currentValue = newValue;
    RefreshButtonValue();
    m_updating = false;
}

void godot::ButtonEditorProperty::RefreshButtonValue() const
{
    m_pPropertyControl->set_pressed(m_currentValue);
}

void godot::ButtonEditorProperty::OnButtonDown()
{
    // Ignore the signal if the property is currently being updated.
    if (m_updating) return;

    m_currentValue = true;
    RefreshButtonValue();
    emit_changed(get_edited_property(), m_currentValue);
}

void godot::ButtonEditorProperty::OnButtonUp()
{
    // Ignore the signal if the property is currently being updated.
    if (m_updating) return;

    m_currentValue = false;
    RefreshButtonValue();
    emit_changed(get_edited_property(), m_currentValue);
}
