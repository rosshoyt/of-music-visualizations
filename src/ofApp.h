#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include <iostream>
#include "MIDIPortState.h"
#include "NoteGridAnimation.h"

class ofApp : public ofBaseApp {

	public:
        ofApp();
		void setup();
		void update();
		void draw();
        void windowResized(int w, int h);
		
private:
        // Class which creates MIDI port and tracks the current state of the midi input's note on and note off's
        MIDIPortState midiPortState;
        // Animation component
        NoteGridAnimation noteGridAnimation;
};
