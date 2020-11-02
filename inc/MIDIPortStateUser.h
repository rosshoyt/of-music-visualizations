#pragma once
#include "MIDIPortState.h"

class MIDIPortStateUser {
public:
    void setMIDIPortState(MIDIPortState* midiPortState) {
        this->midiPortState = midiPortState;
    }
protected:
    MIDIPortState* midiPortState;
};