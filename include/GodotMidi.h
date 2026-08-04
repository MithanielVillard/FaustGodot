#pragma once
#include "MidiHandlerFaust.h"

#include <faust/midi/midi.h>

class IPropertyHandler;

namespace godot { class InputEventMIDI; }

class GodotMidi : public midi_handler
{
public:
    GodotMidi();
    ~GodotMidi() override = default;

    bool startMidi() override;
    void stopMidi() override;

    void OnMidiReceived(godot::InputEventMIDI const* eventMidi);

private:
    godot::MidiHandlerFaust::ListIt m_midiListIt;
};
