#pragma once
#include "ofMain.h"
#include "ofxGui.h"

#include "AnimationComponent.h"
#include "MIDIPortState.h"

extern const int RIGHT_CONTROLBAR;

class NoteGridAnimation : public AnimationComponent {
public:
	NoteGridAnimation(std::string uid, MIDIPortState* midiPortState);

	void setup();
	void update();
	void draw();

    void windowResized(int w, int h);

    // gui components
    ofxPanel gui;

    ofxIntSlider pitchOffsetSlider;
    ofxToggle pitchOffsetUseMIDICCToggle;
    ofParameter<int> pitchOffsetAmount;

    ofxToggle drawLinesToggle;
    ofxToggle drawBackgroundGridToggle;
    ofColor backgroundColor;
    //ofColor backgroundColor;

    ofxColorSlider backgroundColorSelector;
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
    MIDIPortState* midiPortState;

    // stores current width and height of the window
    int windowWidth, windowHeight;

    // gui variables
    bool drawLines;

    void drawBgdGridLines();

    void drawBgdGrid();

    void drawActiveNotes();
};