#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : useVirtualPort(true), virtualMIDIPort("ofxMidiIn Input"), networkMIDIPort("Network Session 1"), notes(), windowWidth(), windowHeight(), nColumns(12), nRows(11), boxWidth(), boxHeight(), drawLines(true), backgroundColor() {

}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowTitle("VS Visual");
    ofEnableAntiAliasing();
    
    setupOfxMIDIPort();
    
    
    
    gui.setup();
    gui.add(drawLinesButton.setup("Draw Lines"));
    gui.add(backgroundColorSelector.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    
    // set global display vars
    windowResized(ofGetWidth(), ofGetHeight());
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    drawNoteGrid();
    drawActiveNotes();
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::drawNoteGrid(){
    
    // calculate box width based on window size
    
    
    // draw black and white background to represent fundamental pitches C - B
    for(int i = 0; i < nColumns; ++i) {
        if((i & 1) == 0){
            ofSetColor(ofColor::paleVioletRed, 50);
            ofDrawRectangle(0, i * boxHeight, windowWidth, boxHeight);
          
        }
        // set color to white for non-accidental keys, black for accidentals
        if(i == 0 || i == 2 || i == 4 || i == 5 || i == 7 || i == 9 || i == 11)
        //if((i > 5 && (i&1)) || ()
            ofSetColor(ofColor::lightGrey, 128);
        else
            ofSetColor(ofColor::darkGrey, 128);
        // draw the column rectangles
        ofDrawRectangle(i * boxWidth, 0, boxWidth, windowHeight);
        // draw border coloumn lines
        drawLine(i * boxWidth, 0, i * boxWidth, windowHeight, ofColor::dimGrey);
    }
    
    
        // draw  horizontal grid lines to represent octaves
        ofSetColor(backgroundColorSelector);
        for(int i = 0; i < nRows; ++i){
            ofDrawLine(0, i * boxHeight, windowWidth, i * boxHeight);
    }
    
}
    
    void ofApp::drawLine(int x, int y, int x2, int y2, ofColor color, int alpha){
        if(drawLinesButton){
            ofSetColor(color, alpha);
            ofDrawLine(x, y, x2, y2);
        }
    }

//--------------------------------------------------------------
void ofApp::drawActiveNotes(){
    // draw all notes currently being played
    auto ns = notes.getAllNotes();
    for(auto note : ns) {
        //std::cout<< "drawing a note!\n";
        int noteNumber = note.first, velocity = note.second;
        int row = nRows - 1 - noteNumber / 12, col = noteNumber  % nColumns;
        // TODO ensure MIDI NOTE #0 doesn't cause issue ( scale midi note #s to start at 1?)
        ofSetColor(velocity * 2,  255 / std::max(1, noteNumber)/*255 / nColumns * col*/, 255 /*255 / nRows * row*/);
        ofDrawRectangle(col * boxWidth, row * boxHeight, boxWidth, boxHeight);
        //std::cout << "col: " << col << ", row: " << row << '\n';
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
    windowWidth = w - RIGHT_CONTROLBAR;
    windowHeight = h;
    
    boxWidth = windowWidth / nColumns;
    boxHeight = windowHeight / nRows;
    
    gui.setPosition(windowWidth, 0);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::setupOfxMIDIPort(){
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

void ofApp::newMidiMessage(ofxMidiMessage& message){
    
    switch(message.status) {
        case MIDI_NOTE_ON:
            //std::cout << "Setting pitch #" << message.pitch << " on, velocity = " << message.velocity  << "\n";
            notes.tryAddNoteOn(message.pitch, message.velocity);
            break;
        case MIDI_NOTE_OFF:
            //std::cout << "Setting pitch #" << message.pitch << " off\n";
            notes.tryAddNoteOff(message.pitch);
            break;
        case MIDI_CONTROL_CHANGE:
            // process MIDI Control Changes
            switch(message.control){
                case 64:
                    switch(message.value){
                        case 0:
                            //std::cout<< "MIDI Control Change # " << message.control << " value = " << message.value << '\n';
                            notes.setSustainPedalOff();
                            break;
                        case 127:
                            //std::cout<< "MIDI Control Change # " << message.control << " value = " << message.value << '\n';
                            notes.setSustainPedalOn();
                            break;
                    }
            }
    }
}
