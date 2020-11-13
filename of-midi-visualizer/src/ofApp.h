#pragma once
#include "ofMain.h"
//#include "ofxGui.h"
#include "ofxDatGui.h"
#include <iostream>
#include "MIDIPortState.h"
#include "MIDIPortStateGUI.h"
#include "GUIComponent.h"
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

class ofApp : public ofBaseApp, public GUIComponent {

	public:
        ofApp();
		void setup();
        void setupGUI() override;
		void update();
		void draw();
        void keyPressed(int key);
        void windowResized(int w, int h);
        void onDropdownEvent(ofxDatGuiDropdownEvent e);
		
private:
        AbletonController abletonController;
        ofxColorSlider backgroundColorSelector;
        
        // Global GUI componenets
        // gui components
        // TODO replace ofxDatGUI
        ofxDatGuiDropdown* animationSelectorDropdown;
        ofxToggle drawAllAnimationsToggle;
       
        std::string currentAnimationUID;
        std::vector<std::string> animationUIDS;
        std::map<std::string, MIDIAnimationComponent*> animationComponentsMap;
        
        // Class which creates MIDI port and tracks the current state of the midi input's note on and note off's
        //MIDIPortStateSettings midiPortSettings;
        //MIDIPortState midiPortState;

        // list of all gui components
        std::vector<GUIComponent*> guiComponentsList;


        // GUI-Only Components (no animation)
        MIDIPortStateGUI midiPortStateGUI;

        // MIDI Animation components
        NoteGridAnimation noteGridAnimation;
        Animated3DMesh animated3DMesh;
        MeshFromImage meshFromImage;
        TexturedSphere texturedSphere;
        ADSRVisualizer adsrVisualizer;
        CircleOfFifths circleOfFifths;  
};
