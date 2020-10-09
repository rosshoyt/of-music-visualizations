#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : midiPortState(4, false), noteGridAnimation("2D Note Grid",&midiPortState), animated3DMesh("3D Mesh",&midiPortState) {}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowTitle("VS Visual");
    ofEnableAntiAliasing();


    // setup animation components
    noteGridAnimation.setup();
    animated3DMesh.setup();
    // setup GUI dropdown animation selector
    animationUIDS.push_back(noteGridAnimation.getUID());
    animationUIDS.push_back(animated3DMesh.getUID());
    // set which animation to use first
    currentAnimationUID = animationUIDS[0];

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
    noteGridAnimation.update();
    animated3DMesh.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (currentAnimationUID == noteGridAnimation.getUID())
        noteGridAnimation.draw();
    else if (currentAnimationUID == animated3DMesh.getUID())
        animated3DMesh.draw();

    animationSelectorDropdown->draw();
}

void ofApp::keyPressed(int key) {
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

