#pragma once
#include "ofMain.h"
#include "ofxGui.h"

#include "AnimationComponent.h"
#include "MIDIPortState.h"

extern const int RIGHT_CONTROLBAR;

class NoteGridAnimation : public AnimationComponent {
public:
	NoteGridAnimation(MIDIPortState* midiPortState, std::string uid = "2D Note Grid");

	void setup();
	void update();
	void draw();
    void drawGUI();

    void windowResized(int w, int h);

    // gui components
    ofxPanel gui;
    ofxIntSlider pitchOffsetSlider;
    ofxToggle pitchOffsetUseMIDICCToggle;
    ofParameter<int> pitchOffsetAmount;

    ofxToggle drawCirclesToggle;


    ofxToggle drawLinesToggle;
    ofxToggle drawBackgroundGridToggle;
    
    ofxColorSlider backgroundColorSelector;
    ofxColorSlider gridLineColorSelector;
    ofxColorSlider octaveRowColorSelector;
    ofxColorSlider noteDisplayColorSelector1;
    ofxColorSlider noteDisplayColorSelector2;

    // array of color sliders for each midi channel
    ofxColorSlider* channelColors;

private:
    // Midi notes # 0 - 127 will be displayed, and there will be 5 extra unused boxes at the bottom right (12 X 11 = 132_
    int nColumns, nRows;

    int boxWidth, boxHeight;
    

    // stores current width and height of the window
    int windowWidth, windowHeight;

    // gui variables
    bool drawLines;

    // Private methods
    void drawBgdGridLines();
    void drawBgdGrid();
    void drawActiveNotes();
};