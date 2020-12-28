#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "MIDIAnimationGUIComponent.h"


//extern const int RIGHT_CONTROLBAR;


struct ChannelSettings {
public:
    ChannelSettings() = default;

    ofxToggle drawCirclesToggle;

    // enum draw_mode { CIRCLES, SQUARES }; //TODO
    //int channelNumber; // TODO ?

};

class NoteGridAnimation : public MIDIAnimationGUIComponent {
public:
	NoteGridAnimation(std::string uid = "2D Note Grid");

	void setup();

    void setupGUI() override;
    void setupParameterListeners() override;
    void update();
	void draw();


    void windowResized(int w, int h);

    // gui components
    ofxToggle drawAllAnimations;
    ofxIntSlider pitchOffsetSlider;
    ofxToggle pitchOffsetUseMIDICCToggle;
    ofParameter<int> pitchOffsetAmount;


    ofxToggle drawLinesToggle;
    ofxToggle drawBackgroundGridToggle;
    
    ofParameter<ofColor> backgroundColorSelector;
    ofxColorSlider gridLineColorSelector;
    ofParameter<ofColor> octaveRowColorSelector;
    ofxColorSlider noteDisplayColorSelector1;
    ofxColorSlider noteDisplayColorSelector2;

    // array of color sliders for each midi channel
    ofxColorSlider* channelColors;

private:
    // list of ofx gui components associated with each midi channel
    std::vector<ChannelSettings*> channelSettingsList;

    // Midi notes # 0 - 127 will be displayed, and there will be 5 extra unused boxes at the bottom right (12 X 11 = 132_
    int nColumns, nRows;

    int boxWidth, boxHeight;
    

    // stores current width and height of the window
    //int windowWidth, windowHeight;

    // gui variables
    bool drawLines;

    // Private methods
    void drawBgdGridLines();
    void drawBgdGrid();
    void drawActiveNotes();
};