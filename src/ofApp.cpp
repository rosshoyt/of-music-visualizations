#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : abletonController(), midiPortState(16, false), noteGridAnimation(&midiPortState, "2D Note Grid"), animated3DMesh(&midiPortState, "3D Mesh"), meshFromImage(&midiPortState, "Mesh From Image") {}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowTitle("VS Visual");
    ofEnableAntiAliasing();

    animationComponents.push_back(&noteGridAnimation);
    animationComponents.push_back(&animated3DMesh);
    animationComponents.push_back(&meshFromImage);
    
    // setup animation components
    for (auto& animation : animationComponents) {
        animation->setup();
        // track UIDS of animations
        animationUIDS.push_back(animation->getUID());
    }
    // set which animation to use first
    currentAnimationUID = animationUIDS[1];

    //// instantiate the animation selector dropdown and set position
    animationSelectorDropdown = new ofxDatGuiDropdown("SELECT AN ANIMATION", animationUIDS);
    animationSelectorDropdown->setPosition(WIDTH, 0);
    //// register to listen for change events
    animationSelectorDropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);
    animationSelectorDropdown->expand();

    // Set ofxGUI menu position for note grid animation
    noteGridAnimation.setMenuXY(WIDTH, HEIGHT / 2);

    // initialize network MIDI port
    midiPortState.setupMIDIPort();
}



//--------------------------------------------------------------
void ofApp::update(){
    animationSelectorDropdown->update();
    for (const auto& animation : animationComponents) {
        if (animation->getUID() == currentAnimationUID) {
            animation->update();
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    for (const auto& animation : animationComponents) {
        if (animation->getUID() == currentAnimationUID) {
            animation->draw();
        }
    }

    animationSelectorDropdown->draw();
}

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
    noteGridAnimation.windowResized(w, h);
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
    currentAnimationUID = animationUIDS[e.child];
    animationSelectorDropdown->expand();
}

