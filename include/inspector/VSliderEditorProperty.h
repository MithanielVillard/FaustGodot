#pragma once

#include <godot_cpp/classes/editor_property.hpp>

namespace godot
{
    class VSlider;

    class VSliderEditorProperty : public EditorProperty
    {
    GDCLASS(VSliderEditorProperty, EditorProperty)

    public:
        void setup(float min, float max, float step);
        void _update_property() override;

    protected:
        static void _bind_methods();
        void RefreshSliderValue() const;

        void OnValueChange(float value);

    private:
        VSlider* m_pPropertyControl { nullptr};
        double m_currentValue { 0 };
        bool m_updating { false };
    };

}
