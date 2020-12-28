#pragma once
#include <string>
#include "ofMain.h"
#include "Utils.h"
#include "MIDIAnimationGUIComponent.h"


class CircleOfFifths : public MIDIAnimationGUIComponent {
public:
    CircleOfFifths(std::string uid = "Circle of Fifths");

    void setup();
    void setupGUI() override;
    void setupParameterListeners();
    void update();
    void draw();

private:
    void drawHaloAroundCursor();
    void drawHaloAroundPoint(ofVec2f point, int channelNum = -1);

    int numOctaves = 10;
    int numPitches = 12;

    int alpha = 150;    
    
    ofParameter<bool> drawOctavesToggle;
    ofParameter<float> noteCircleSizeSlider;
    ofParameter<float> octaveSizeMultiplierSlider;
    ofParameter<int> noteMultiplierSlider;
    ofParameter<int> pitchOffsetSlider;
    ofParameter<bool> drawHaloOnNotesToggle;
};