#pragma once

#include <list>
#include <functional>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/input_event.hpp>

namespace godot
{
    class InputEventMIDI;

    class MidiHandlerFaust : public Node
    {
    GDCLASS(MidiHandlerFaust, Node)

    public:
        using MidiCallback = std::function<void(InputEventMIDI*)>;
        using ListIt = std::list<MidiCallback>::iterator;

        MidiHandlerFaust() = default;
        ~MidiHandlerFaust() override = default;

        void _ready() override;

        void _input(const Ref<InputEvent>& p_event) override;
        void _exit_tree() override;

        static std::list<MidiCallback> MidiCallbacks;

    protected:
        static void _bind_methods();

        void HandleMidiInput(InputEventMIDI* midiEvent);
    };

}

