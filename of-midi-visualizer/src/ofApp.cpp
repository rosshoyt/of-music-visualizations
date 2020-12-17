#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : abletonController() {}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetWindowTitle("OpenFrameworks MIDI Visualizer - Ross Hoyt Music");
    ofEnableAntiAliasing();

    // add MIDI Port State GUI and Main GUI to the permanent gui components list
    permanentGUIComponentsList.push_back(&midiPortState);
    permanentGUIComponentsList.push_back(&mainGUI);
    // setup all GUI components which are not animations
    for (auto& guiComponent : permanentGUIComponentsList) {
        guiComponent->setupGUI();
        // also add them to the list for all GUI components for updating positions on resize
        guiComponentsList.push_back(guiComponent);
    }

    // track animation components 
    animationComponentsList.push_back(&noteGridAnimation);
    animationComponentsList.push_back(&animated3DMesh);
    animationComponentsList.push_back(&adsrVisualizer);
    animationComponentsList.push_back(&circleOfFifths);
    //animationComponentsList.push_back(&meshFromImage);

    // setup animation components
    for (auto& component : animationComponentsList) {
        component->setMIDIPortState(&midiPortState);
        component->setup();
        component->setupGUI();
        // add a toggle to turn the animation on and off from the main gui
        mainGUI.addToggle(component->getUID(), false);
        // add the component to the GUI Component list (TODO separate MIDIAnimationComponent from GUIComponent parent class)
        guiComponentsList.push_back(component);
    }

    // initialize the animation toggles in the main gui
    mainGUI.initToggles();
    
    // set an animation to start with
    mainGUI.setToggleState(circleOfFifths.getUID(), true);
    
    // load all GUI panel settings 
    for (auto& guiComponent : guiComponentsList) {
        guiComponent->loadSavedSettings();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    for (auto& animation : animationComponentsList) {
        if(mainGUI.isToggled(animation->getUID()))
            animation->update();
    }    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(mainGUI.getBackgroundColor());
    for (auto& animation: animationComponentsList) {
        if (mainGUI.isToggled(animation->getUID())) {
            animation->draw();
            animation->drawGUI();
        }
    }
    for (auto& guiComponent : permanentGUIComponentsList) {
        guiComponent->drawGUI();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case ' ':
        std::cout << "Pressed Space\n";
        abletonController.processSpacebar();
    }
    animated3DMesh.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    float animationDisplayWidth = w - RIGHT_CONTROLBAR;
    
    // update all animation posisions
    for (auto& animation : animationComponentsList) {
        animation->setAnimationDimensions(animationDisplayWidth, h);
        animation->resized(w, h);
    }


    // TODO Algorithm to sort GUI panels most evenly across a given number of columns of fixed size

    // or

    // TODO Algorithm to sort dynamically sort GUI panels in a 



    // update all GUI panel positions
    float guiSpacer = 10; // TODO move to global variable, create WindowManager
    float x = animationDisplayWidth, y = 0;
    for (auto& component : guiComponentsList) {
        auto bottomOfNewGUIComponent = y + component->getMenuHeight();
        // check if we are going to position GUI off-screen
        if (bottomOfNewGUIComponent > HEIGHT) {
            // TODO refactor the positioning of the GUI components
            y = 0; // reset Y
            x += OFXGUI_DEF_WIDTH; // shift right to the next column 
            // TODO ensure doesn't paint gui menu in off-screen columns
        }
        component->setMenuXY(x, y);
        y += component->getMenuHeight() + guiSpacer; 
    }
}

