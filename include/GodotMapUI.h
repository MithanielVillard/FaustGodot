#pragma once
#include <faust/gui/MapUI.h>

class IPropertyHandler;

class GodotMapUI : public MapUI
{
public:

    GodotMapUI(IPropertyHandler& effectRef);
    ~GodotMapUI() override = default;

    void addButton(const char* label, float* zone) override;
    void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override;
    void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override;
    void addCheckButton(const char *label, float *zone) override;

private:
    IPropertyHandler& m_pPropertyHandler;
};
