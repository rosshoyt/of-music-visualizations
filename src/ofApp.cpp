#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : useVirtualPort(true), virtualMIDIPort("ofxMidiIn Input"), networkMIDIPort("Network Session 1"), notes() {

}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowTitle("VS Visual");
    ofEnableAntiAliasing();
    
    if(useVirtualPort) {
        std::cout << "Setting up local virtual midi port " << virtualMIDIPort << "\n";
        midiIn.openVirtualPort();
        std::cout<< virtualMIDIPort <<" port # is "<< midiIn.getPort() << '\n';
    }
    else {
        for(std::string portIn : midiIn.getInPortList()) {
            std::cout << "Available InPort: " << portIn << '\n';
            if(portIn.compare(networkMIDIPort) == 0){
                midiIn.openPort(networkMIDIPort);
                if(midiIn.isOpen())
                    std::cout << "Found desired network port and opened it \n";
            }
        }
        
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
    
    
    auto ns = notes.getNotes();
    for(auto note : ns) {
        int noteNumber = note.first, velocity = note.second;
        int row = noteNumber / 12, col = noteNumber % nColumns;
        ofSetColor(velocity * 2, 255, 255);
        ofDrawRectangle(col * boxWidth, row * boxHeight, boxWidth, boxHeight);
        // std::cout << col << ", " << row << '\n';
    }

    

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
        std::cout << "Setting pitch #" << message.pitch << " on, velocity = " << message.velocity  << "\n";
        notes.tryAddNoteOn(message.pitch, message.velocity);
    } else if(message.status == MIDI_NOTE_OFF){
        std::cout << "Setting pitch #" << message.pitch << " off\n";
        notes.tryAddNoteOff(message.pitch);
    }
}
