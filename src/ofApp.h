#pragma once
#include "ofMain.h"
//#include "ofxGui.h"
#include "ofxDatGui.h"
#include <iostream>
#include "MIDIPortState.h"
#include "NoteGridAnimation.h"
#include "Animated3DMesh.h"
#include "MeshFromImage.h"
#include "AbletonController.h"
#include "AnimationComponent.h"

extern const int WIDTH, HEIGHT;

class ofApp : public ofBaseApp, AbletonController {

	public:
        ofApp();
		void setup();
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
        ofxPanel gui;
        // TODO replace ofxDatGUI
        ofxDatGuiDropdown* animationSelectorDropdown;
        std::string currentAnimationUID;
        std::vector<std::string> animationUIDS;
        
        // Class which creates MIDI port and tracks the current state of the midi input's note on and note off's
        MIDIPortState midiPortState;


        // Animation components
        NoteGridAnimation noteGridAnimation;
        Animated3DMesh animated3DMesh;
        MeshFromImage meshFromImage;
        std::vector<AnimationComponent*> animationComponents;
};
