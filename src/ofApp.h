#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMidi.h"
#include <iostream>
#include "MIDIPortState.h"

#define HEIGHT 1024
#define WIDTH 1024
#define RIGHT_CONTROLBAR 300

class ofApp : public ofBaseApp {

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
    
        // gui components
        ofxPanel gui;
    
        ofxIntSlider pitchOffsetSlider;
        ofxToggle pitchOffsetUseMIDICCToggle;
        ofParameter<int> pitchOffsetAmount;
    
        ofxToggle drawLinesToggle;
        ofxToggle drawBackgroundGridToggle;
        ofColor backgroundColor;
        
        ofxColorSlider gridLineColorSelector;
        ofxColorSlider octaveRowColorSelector;
        ofxColorSlider noteDisplayColorSelector1;
        ofxColorSlider noteDisplayColorSelector2;
    
        // array of color sliders for each midi channel
        ofxColorSlider* channelColors;
private:
        // flag set to true if using local MIDI port, false if using network MIDI port
        bool useVirtualPort;
        // names of local and network MIDI ports.
        std::string virtualMIDIPort, networkMIDIPort;
        // Midi notes # 0 - 127 will be displayed, and there will be 5 extra unused boxes at the bottom right (12 X 11 = 132_
        int nColumns, nRows;
    
        int boxWidth, boxHeight;
    
    
        // Class which creates MIDI port and tracks the current state of the midi input's note on and note off's
        MIDIPortState midiPortState;
    
        // stores current width and height of the window
        int windowWidth, windowHeight;
    
        // gui variables
        bool drawLines;
    
        /**
         * Method which sets up the OFX MIDI port. Creates a 'local' virtual port if field
         * useVirtualPort is set to true; otherwise, creates a network MIDI port
         */
        void setupOfxMIDIPort();
    
        void drawBgdGridLines();
    
        void drawBgdGrid();
    
        void drawActiveNotes();
    
};
