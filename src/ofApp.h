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
        // Midi Input port
        ofxMidiIn midiIn;
        // flag set to true if using local MIDI port, false if using network MIDI port
        bool useVirtualPort;
        // names of local and network MIDI ports.
        std::string virtualMIDIPort, networkMIDIPort;
    
        // Class storing the current state of the midi input's note on and note off's.
        MIDIChannelNotesState notes;
    
        
        // stores current width and height of the window
        int windowWidth, windowHeight;
    
        /**
         * Method which sets up the OFX MIDI port. Creates a 'local' virtual port if field
         * useVirtualPort is set to true; ot herwise, creates a network MIDI port
         */
        void setupOfxMIDIPort();
    
        /**
         * ofMidiListener implemented method
         */
        void newMidiMessage(ofxMidiMessage& message);
    
        /**
         * Method which draws a piano background and visualizes the notes currently
         * being played
         * Called from draw()
         */
        void drawNoteGrid();
};
