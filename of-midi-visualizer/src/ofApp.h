#pragma once
#include "ofMain.h"
#include <iostream>
#include "GUIComponent.h"
#include "MIDIPortState.h"
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

extern const int ANIMATION_WIDTH, ANIMATION_HEIGHT, OFXGUI_DEF_WIDTH, RIGHT_CONTROLBAR;

class ofApp : public ofBaseApp {

	public:
        ofApp();
		void setup();
		void update();
		void draw();
        void keyPressed(int key);
        void windowResized(int w, int h);
		
private:
        // Class which communicates with Ableton via OSC
        AbletonController abletonController;

        // Individual MIDI Animations
        NoteGridAnimation noteGridAnimation;
        Animated3DMesh animated3DMesh;
        MeshFromImage meshFromImage;
        TexturedSphere texturedSphere;
        ADSRVisualizer adsrVisualizer;
        CircleOfFifths circleOfFifths;
        // List for all MIDI Animations
        std::vector<MIDIAnimationComponent*> animationComponentsList;

        // GUI Components of the main window (always visible)
        MIDIPortState midiPortState;
        MainGUI mainGUI;

        // List for always-visible GUI components
        std::vector<GUIComponent*> permanentGUIComponentsList;
        
        // List for all GUI components
        std::vector<GUIComponent*> guiComponentsList;     
};
