#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include <iostream>
#include "MidiNotesState.h"

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
        std::string desiredNetworkPort;
        MidiNotesState notes;
    
        void newMidiMessage(ofxMidiMessage& message);
};
