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
    
    currentAnimationUID = animationUIDS[0];

    // instantiate the dropdown //
    animationSelectorDropdown = new ofxDatGuiDropdown("SELECT AN ANIMATION", animationUIDS);

    // and position it in the middle of the screen //
    animationSelectorDropdown->setPosition(WIDTH, 0);

    // let's set the stripe of each option to its respective color //
    //for (int i = 0; i < animationSelectorDropdown->size(); i++) animationSelectorDropdown->getChildAt(i)->setStripeColor(colors[i]);

    // register to listen for change events //
    animationSelectorDropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);

    // finally let's have it open by default //
    animationSelectorDropdown->expand();

    // Set Sub Menu's position based on main menu size
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

