#pragma once
#include <string>
#include "ofMain.h"
#include "Utils.h"
#include "MIDIAnimationComponent.h"


class CircleOfFifths : public MIDIAnimationComponent {
public:
    CircleOfFifths(std::string uid = "Circle of Fifths");

    void setup();
    void setupGUI() override;
    void update();
    void draw();

private:
    void drawHaloAroundCursor();
    void drawHaloAroundPoint(ofVec2f point);

    int numOctaves = 10;
    int numPitches = 12;

    int alpha = 150;    
    
    ofParameter<bool> drawOctavesToggle;
    ofParameter<bool> drawChromaticModeToggle;
    ofParameter<bool> drawHaloOnNotesToggle;
    ofParameter<int> noteMultiplierToggle;
    ofParameter<float> noteCircleSizeSlider;
    ofParameter<float> octaveSizeMultiplierSlider;
    ofxColorSlider noteColorSlider;
    //ofParameter<ofColor> noteColor;
    ofParameter<int> pitchOffsetSlider;

    // TODO create class to manage a GUI toggle for enum values (stepped slider)
    struct Toggle {
        Toggle(std::vector<std::string> _states) : states(_states) {
            currentState.set("States", 0, 0, states.size() - 1);
        }
        
        ofParameter<int> currentState;
        ofParameter<std::string> currentStateText;
        std::vector<std::string> states;
        
        std::string getCurrentState() {

        }

        // void draw(){} // TODO implement?
    };
};