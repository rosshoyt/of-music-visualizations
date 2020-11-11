#pragma once
#include "MIDIAnimationComponent.h"
#include "Utils.h"
#include "ofMain.h"


class CircleOfFifths : public MIDIAnimationComponent {
public:
    CircleOfFifths(std::string uid = "Circle of Fifths");

    void setup();
    void setupGUI() override;
    void update();
    void draw();
    void drawHaloAroundCursor();

private:
    int numOctaves = 10;
    int numPitches = 12;

    int alpha = 150;
};