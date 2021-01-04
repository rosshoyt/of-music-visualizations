#pragma once
#include "ofMain.h"
#include <iostream>
#include <queue>
#include "GUIComponent.h"
#include "MIDIPortState.h"
#include "MainGUI.h"
#include "NoteGridAnimation.h"
#include "MIDIAnimationGUIComponent.h"
#include "Animated3DMesh.h"
#include "MeshFromImage.h"
#include "TexturedSphere.h"
#include "AbletonController.h"  
#include "ADSRVisualizer.h"
#include "LFOVisualizer.h"
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
        void mouseScrolled(int x, int y, float scrollX, float scrollY);
        void windowResized(int w, int h);
		
private:
        
        void repositionGUIComponents(float yOffset = 0);
        

        // variables used to support scrolling in the GUI panel area
        // TODO create GUIManager class
        float guiMenuYOffset = 0, scrollSpeed = 35;
        const float GUI_MENU_Y_MAX_OFFSET = ANIMATION_HEIGHT;
      
        
        
        
        // Class which communicates with Ableton via OSC
        // TODO create GUI controls for this component
        AbletonController abletonController;

        // MIDI Animation Components
        NoteGridAnimation noteGridAnimation;
        Animated3DMesh animated3DMesh;
        MeshFromImage meshFromImage;
        TexturedSphere texturedSphere;
        CircleOfFifths circleOfFifths;

        // MIDI Animations which are tests of custom library components
        // TODO convert to GUIAnimation class, as these don't use MIDI Port
        // TODO will need to move 'isToggled' to AnimationComponent.h from GUIComponent.h
        ADSRVisualizer adsrVisualizer;
        LFOVisualizer lfoVisualizer;

        // Always-Visible GUI Components
        MIDIPortState midiPortState;

        // Always-visible GUI Animation Components
        MainGUI mainGUI;

        // List for all MIDI Animations
        std::vector<MIDIAnimationGUIComponent*> midiAnimationGUIComponentsList;

        // List for all GUI components
        std::vector<GUIComponent*> guiComponentsList;

        // List for always-visible GUI-only components
        std::vector<GUIComponent*> permanentGUIComponentsList;

        // List for always-visible GUI Animation Components
        std::vector<GUIAnimationComponent*> permanentGUIAnimationComponentsList;
};
