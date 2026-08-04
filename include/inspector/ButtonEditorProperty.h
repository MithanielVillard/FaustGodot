#pragma once
#include <godot_cpp/classes/editor_property.hpp>

namespace godot
{
    class Button;

    class ButtonEditorProperty : public EditorProperty
    {
        GDCLASS(ButtonEditorProperty, EditorProperty)

        public:
        void setup();
        void _update_property() override;

    protected:
        static void _bind_methods();
        void RefreshButtonValue() const;

        void OnButtonDown();
        void OnButtonUp();

    private:
        Button* m_pPropertyControl { nullptr};
        bool m_currentValue { false };
        bool m_updating { false };
    };

}