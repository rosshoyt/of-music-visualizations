#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : midiPortState(4, false), noteGridAnimation(&midiPortState) {}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowTitle("VS Visual");
    ofEnableAntiAliasing();

    noteGridAnimation.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    noteGridAnimation.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    noteGridAnimation.draw();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    noteGridAnimation.windowResized(w, h);
}

