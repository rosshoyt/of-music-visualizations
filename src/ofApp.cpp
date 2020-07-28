#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : desiredNetworkPort("Network Session 1"), notes() {

}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowTitle("VS Visual");
    ofEnableAntiAliasing();
    
    if(midiIn.getInPortList().size() > 0){
        for(std::string portIn : midiIn.getInPortList()) {
            std::cout << "Available InPort: " << portIn << '\n';
            if(portIn.compare(desiredNetworkPort) == 0){
                midiIn.openPort(desiredNetworkPort);
                if(midiIn.isOpen())
                    std::cout << "Found desired network port and opened it \n";
            }
        }
        
    }
    else {
        midiIn.openVirtualPort("ofxMidiIn Input");
        std::cout<<"ofxMidiIn port # is "<< midiIn.getPort() << '\n';
    }
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
    
    
    std::vector<int> ns = notes.getNotes();
    for(auto noteNumber : ns) {
        int row = noteNumber / 12, col = noteNumber % nColumns;
        //ofTranslate(col * boxWidth, row * boxHeight);
        
        ofDrawRectangle(col * boxWidth, row * boxHeight, boxWidth, boxHeight);
        std::cout << col << ", " << row << '\n';
    }
    
//    for(auto noteNumber : noteOnSet){
//
//        int row = noteNumber / 12, col = noteNumber % nColumns;
//        //ofTranslate(col * boxWidth, row * boxHeight);
//
//        ofDrawRectangle(col * boxWidth, row * boxHeight, boxWidth, boxHeight);
//        std::cout << col  << ", " << row << '\n';
//    }
//
    

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
        notes.tryAddNoteOn(message.pitch);
    } else if(message.status == MIDI_NOTE_OFF){
        std::cout << "Setting pitch #" << message.pitch << " off\n";
        notes.tryAddNoteOff(message.pitch);
    }
}
