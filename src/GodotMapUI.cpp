#include "GodotMapUI.h"
#include "AudioEffectFaust.h"

#include <format>

using namespace godot;


#define REGISTER_PROPERTY(property)                                 \
StringName striped_label(label);                                    \
striped_label = striped_label.replace_char('/', '|');               \
                                                                    \
for (PropertyInfo& info : m_pPropertyHandler.GetPropertyList())     \
    if (info.name == striped_label) return;                         \
                                                                    \
addZoneLabel(String(striped_label).utf8().get_data(), zone);        \
m_pPropertyHandler.AddProperty(property);                           \



GodotMapUI::GodotMapUI(IPropertyHandler& effectRef) : m_pPropertyHandler(effectRef) {}

void GodotMapUI::addButton(const char* label, float* zone)
{
    REGISTER_PROPERTY(
        PropertyInfo(
        Variant::CALLABLE,
        striped_label,
        PROPERTY_HINT_TOOL_BUTTON)
    );
}

void GodotMapUI::addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{
    REGISTER_PROPERTY(
        PropertyInfo(
        Variant::FLOAT,
        striped_label,
        PROPERTY_HINT_RANGE,
        std::format("{},{},{},vertical", min, max, step).c_str())
    );
    m_pPropertyHandler.SetProperty(striped_label, init);  //Set initial value of parameter
}

void GodotMapUI::addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{
    REGISTER_PROPERTY(
         PropertyInfo(
         Variant::FLOAT,
         striped_label,
         PROPERTY_HINT_RANGE,
         std::format("{},{},{}", min, max, step).c_str())
    );
    m_pPropertyHandler.SetProperty(striped_label, init);  //Set initial value of parameter
}

void GodotMapUI::addCheckButton(const char *label, float *zone)
{
    REGISTER_PROPERTY(
      PropertyInfo(
      Variant::BOOL,
      striped_label)
  );
}
