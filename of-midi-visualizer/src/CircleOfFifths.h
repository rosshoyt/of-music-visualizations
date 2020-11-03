#pragma once
#include "MIDIAnimationComponent.h"
#include "Utils.h"
#include "ofMain.h"


class CircleOfFifths : public MIDIAnimationComponent {
public:
    CircleOfFifths(std::string uid = "Circle of Fifths") : MIDIAnimationComponent(uid) {

    }
    // method which should be called to initialize the AnimationComponent and prepare it to display at some point in the future.
    // memory allocations, expensive 1-time  initializations and file operations are examples that should be implemented in setup()
    void setup() {
    } 

    void update() {

    }

    void draw() {

        //drawHaloAroundCursor();
        
        // TODO add 'snap to cursor' control so that Circle of Fifths displays around cursor?
        //ofVec2f centerPos(ofGetMouseX(), ofGetMouseY());
        ofVec2f centerPos(animationWidth / 2.f, animationHeight / 2.f);
        float smallestDim = animationWidth > animationHeight ? animationHeight : animationWidth;
        
        for (auto channel: midiPortState->getAllChannelNotes()) {
            for (auto noteVel : channel) {
                auto octavePitch = utils::getOctavePitchPair(noteVel.first);

                float rads = 2 * utils::pi * octavePitch.second / numPitches; // The rotate function uses degrees!

                float radius = animationHeight / 2.f / numOctaves * (octavePitch.first + 1);

                auto point = centerPos + ofVec2f(radius * std::cos(rads), radius * std::sin(rads));

                ofColor aqua(0, 252, 255, alpha);
                ofColor purple(198, 0, 205, alpha);
                ofColor inbetween = aqua.getLerped(purple, float(octavePitch.second) / 12.f);

                ofSetColor(inbetween);

                float circleSize = 40 * noteVel.second / 127.f;

                ofDrawCircle(point, circleSize);


            }
        }
   
    }

    void drawHaloAroundCursor() {
        // Triangle Brush Source: https://github.com/openframeworks/ofBook/blob/master/chapters/intro_to_graphics/code/1_ii_e_Triangle_Brush/src/ofApp.cpp
       // Code for the final version of the brush

        int numTriangles = 10;
        int minOffset = 5;
        int maxOffset = 70;
        int alpha = 150;

        for (int t = 0; t < numTriangles; ++t) {
            float offsetDistance = ofRandom(minOffset, maxOffset);

            ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());

            // Define a triangle at the origin (0,0) that points to the right
            ofVec2f p1(0, 6.25);
            ofVec2f p2(25, 0);
            ofVec2f p3(0, -6.25);

            float rotation = ofRandom(360); // The rotate function uses degrees!
            p1.rotate(rotation);
            p2.rotate(rotation);
            p3.rotate(rotation);

            ofVec2f triangleOffset(offsetDistance, 0.0);
            triangleOffset.rotate(rotation);

            p1 += mousePos + triangleOffset;
            p2 += mousePos + triangleOffset;
            p3 += mousePos + triangleOffset;

            ofColor aqua(0, 252, 255, alpha);
            ofColor purple(198, 0, 205, alpha);
            ofColor inbetween = aqua.getLerped(purple, ofRandom(1.0));
            ofSetColor(inbetween);

            ofDrawTriangle(p1, p2, p3);
        }
        // Alternate code, for the rotating triangle brush discussed in the chapter:

//        ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());
//
//        // Define a triangle at the origin (0,0) that points to the right
//        ofVec2f p1(0, 25.0);
//        ofVec2f p2(100, 0);
//        ofVec2f p3(0, -25.0);
//
//        // Rotate the triangle points around the origin
//        float rotation = ofRandom(360); // Uses degrees!
//        p1.rotate(rotation);
//        p2.rotate(rotation);
//        p3.rotate(rotation);
//
//        // Shift the triangle to the mouse position
//        p1 += mousePos;
//        p2 += mousePos;
//        p3 += mousePos;
//
//        ofSetColor(255, 50);
//        ofDrawTriangle(p1, p2, p3);
    //}

    //// If the right mouse button is pressed...
    //if (ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)) {
    //    ofBackground(0);  // Erase the screen with a black background
    //}
    }

    void drawGUI() {

    }
private:
    int numOctaves = 10;
    int numPitches = 12;

    int alpha = 150;
};