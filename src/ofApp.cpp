#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : noteOnSet(), noteOns(128, false) {

}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowTitle("VS Visual");
    ofEnableAntiAliasing();
    
    midiIn.openVirtualPort("ofxMidiIn Input");
    midiIn.addListener(this);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    // we'll only display the midi notes # 0 - 127, and there will be 5 extra unused boxes at the bottom right.
    int nColumns = 12, nRows = 11;
    
    int boxWidth = ofGetWidth() / nColumns, boxHeight = ofGetHeight() / nRows;
    
    
    //if(noteOnSet.size() > 0) ofDrawRectangle(0, 0, ofGetWidth(), ofGetWidth());
    
    for(auto noteNumber : noteOnSet){
        int row = noteNumber / 12, col = noteNumber % nColumns;
        //ofTranslate(col * boxWidth, row * boxHeight);
        
        ofDrawRectangle(col * boxWidth, row * boxHeight, boxWidth, boxHeight);
        std::cout << col << ", " << row << '\n';
    }
    
    
    //    std::cout<< "Screen Width: "<< ofGetWidth() << " Screen Height: " << ofGetHeight() << '\n';
//    std::cout<< "Box Width: " << boxWidth << " Box Height: " << boxHeight << '\n';
//    for(int i = 0; i <= nHorizontalBoxes; i++){
//
//        for(int j = 0; j <= nVerticalBoxes; j++){
//            //ofTranslate(i*boxWidth, j*boxHeight);
//            int noteIndex = i * j - 1;
//            if(noteOns[noteIndex]){
//                std::cout << "NoteOn! Drawing rectangle in cell " << i << ' ' << j << " (note index # " <<  noteIndex << '\n';
//                ofDrawRectangle(0, 0, boxWidth, boxHeight);
//            }
//        }
//    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


void ofApp::newMidiMessage(ofxMidiMessage& message){
    if(message.status == MIDI_NOTE_ON ) {
        std::cout << "Setting pitch #" << message.pitch << " on\n";
        //noteOns[message.pitch] = true;
        noteOnSet.insert(message.pitch);
    }else if(message.status == MIDI_NOTE_OFF){
        std::cout << "Setting pitch #" << message.pitch << " off\n";
        //noteOns[message.pitch] = false;
        noteOnSet.erase(message.pitch);
    }
}
