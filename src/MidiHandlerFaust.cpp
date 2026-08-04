#include "MidiHandlerFaust.h"

#include <algorithm>
#include <godot_cpp/classes/input_event_midi.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

std::list<MidiHandlerFaust::MidiCallback> MidiHandlerFaust::MidiCallbacks {};

void MidiHandlerFaust::_ready()
{
    OS::get_singleton()->open_midi_inputs();
    UtilityFunctions::print("Midi inputs : ", OS::get_singleton()->get_connected_midi_inputs());
}

void MidiHandlerFaust::_input(const Ref<InputEvent>& p_event)
{
    if (!p_event->is_class("InputEventMIDI")) return;

    InputEventMIDI* event = static_cast<InputEventMIDI*>(p_event.ptr());

    //HandleMidiInput(event);

    std::ranges::for_each(MidiCallbacks, [&](MidiCallback const& callback) { callback(event); });
    emit_signal("midi_received", event);
}

void MidiHandlerFaust::_exit_tree()
{
    OS::get_singleton()->close_midi_inputs();
}

void MidiHandlerFaust::HandleMidiInput(InputEventMIDI* midiEvent)
{
    UtilityFunctions::print("Channel : ", midiEvent->get_channel());
    UtilityFunctions::print("Note velocity : ", midiEvent->get_velocity());
    UtilityFunctions::print("Note pitch : ", midiEvent->get_pitch());
}

void MidiHandlerFaust::_bind_methods()
{
    ADD_SIGNAL(MethodInfo("midi_received", PropertyInfo(Variant::OBJECT, "MidiEvent")));
}



