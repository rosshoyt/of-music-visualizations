#pragma once
#include "MIDIAnimationComponent.h"
#include "Utils.h"
#include "ofMain.h"


class CircleOfFifths : public MIDIAnimationComponent {
public:
    CircleOfFifths(std::string uid = "Circle of Fifths");
    // method which should be called to initialize the AnimationComponent and prepare it to display at some point in the future.
    // memory allocations, expensive 1-time  initializations and file operations are examples that should be implemented in setup()
    void setup();

    void update();

    void draw();

    void drawHaloAroundCursor();

    void drawGUI();
private:
    int numOctaves = 10;
    int numPitches = 12;

    int alpha = 150;
};