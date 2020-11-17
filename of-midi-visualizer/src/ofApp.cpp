#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : abletonController() {}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetWindowTitle("OpenFrameworks MIDI Visualizer - Ross Hoyt Music");
    ofEnableAntiAliasing();

    // setup the main GUI
    mainGUI.setupGUI();
    
    // track animation components 
    animationComponentsMap.push_back(&noteGridAnimation);
    animationComponentsMap.push_back(&animated3DMesh);
    animationComponentsMap.push_back(&adsrVisualizer);
    animationComponentsMap.push_back(&circleOfFifths);
    //animationComponentsMap.insert(&meshFromImage);

    // setup animation components
    for (auto& component : animationComponentsMap) {
        component->setMIDIPortState(&mainGUI.midiPortStateGUI.getMIDIPortState());
        component->setup();
        component->setupGUI();
        mainGUI.addToggle(component->getUID(), false);
    }
    // setup the animation toggles in the main gui
    mainGUI.initToggles();

    // force window resize
    windowResized(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    for (auto& animation : animationComponentsMap) {
        if(mainGUI.isToggled(animation->getUID()))
            animation->update();
    }    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(mainGUI.getBackgroundColor());
    for (auto& animation: animationComponentsMap) {
        if (mainGUI.isToggled(animation->getUID())) {
            animation->draw();
            animation->drawGUI();
        }
    }
    mainGUI.drawGUI();
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
    float animationWidth = w - RIGHT_CONTROLBAR;
    // update all animation posisions
    for (auto& animation : animationComponentsMap) {
        std::cout << "Resizing animation " << animation->getUID() << "\n";      
        animation->setAnimationDimensions(animationWidth, h);
        // TODO have animation component automatically resize?
        animation->resized(w, h);
    }

    // update gui positions
    // start with main gui
    mainGUI.setMenuXY(animationWidth, 0);
    float currX = animationWidth, currY = mainGUI.getMenuHeight();
    for (auto& component : animationComponentsMap) {
        auto guiCompHeight = component->getMenuHeight();
        if (currY >= HEIGHT) {
            currX += OFXGUI_DEF_WIDTH;
            currY = 0;
        }
        component->setMenuXY(currX, currY);
        currY += guiCompHeight;
    }
}

