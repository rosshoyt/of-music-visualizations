#pragma once
#include "MIDIPortState.h"

class MIDIPortStateUser {
public:
    void setMIDIPortState(MIDIPortState* midiPortState);
protected:
    MIDIPortState* midiPortState;
};