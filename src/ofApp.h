#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include <iostream>
#include "MIDIChannelNotesState.h"

class ofApp : public ofBaseApp, ofxMidiListener{

	public:
        ofApp();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
private:
        
        ofxMidiIn midiIn;
        // flag set to true if using local MIDI port, false if using network MIDI port
        bool useVirtualPort;
        // names of local and network MIDI ports.
        std::string virtualMIDIPort, networkMIDIPort;
        MIDIChannelNotesState notes;
    
        // methods
        void newMidiMessage(ofxMidiMessage& message);
    
        // display methods
        void drawNoteGrid();
};
