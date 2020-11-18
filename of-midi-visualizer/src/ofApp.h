#pragma once
#include "ofMain.h"
#include <iostream>
#include "GUIComponent.h"
#include "MIDIPortState.h"
#include "MIDIPortStateGUI.h"
#include "MainGUI.h"
#include "NoteGridAnimation.h"
#include "MIDIAnimationComponent.h"
#include "Animated3DMesh.h"
#include "MeshFromImage.h"
#include "TexturedSphere.h"
#include "AbletonController.h"  
#include "ADSRVisualizer.h"
#include "CircleOfFifths.h"
#include "Utils.h"

extern const int WIDTH, HEIGHT, OFXGUI_DEF_WIDTH, RIGHT_CONTROLBAR;

class ofApp : public ofBaseApp {

	public:
        ofApp();
		void setup();
		void update();
		void draw();
        void keyPressed(int key);
        void windowResized(int w, int h);
		
private:
        // Individual MIDI Animations
        NoteGridAnimation noteGridAnimation;
        Animated3DMesh animated3DMesh;
        MeshFromImage meshFromImage;
        TexturedSphere texturedSphere;
        ADSRVisualizer adsrVisualizer;
        CircleOfFifths circleOfFifths;
        
        // All MIDI Animations
        std::vector<MIDIAnimationComponent*> animationComponentsList;

        // Individual GUI Components
        MainGUI mainGUI;

        // All GUI components
        std::vector<GUIComponent*> guiComponentsList;

        // Class which communicates with Ableton via OSC
        AbletonController abletonController;
};
