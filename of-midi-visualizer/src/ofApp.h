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

extern const int WIDTH, HEIGHT, RIGHT_CONTROLBAR;

class ofApp : public ofBaseApp {

	public:
        ofApp();
		void setup();
		void update();
		void draw();
        void keyPressed(int key);
        void windowResized(int w, int h);
		
private:
        AbletonController abletonController;
        
        
        // main gui component (always shown)
        MainGUI mainGUI;
        
        // list for all MIDI Animation Components
        std::vector<MIDIAnimationComponent*> animationComponentsMap;

        // MIDI Animation components
        NoteGridAnimation noteGridAnimation;
        Animated3DMesh animated3DMesh;
        MeshFromImage meshFromImage;
        TexturedSphere texturedSphere;
        ADSRVisualizer adsrVisualizer;
        CircleOfFifths circleOfFifths;  
};
