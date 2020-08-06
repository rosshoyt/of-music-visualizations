#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : useVirtualPort(false ), virtualMIDIPort("ofxMidiIn Input"), networkMIDIPort("Network Session 1"), notes(), windowWidth(), windowHeight(), nColumns(12), nRows(11), boxWidth(), boxHeight(), drawLines(true), backgroundColor() {

}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowTitle("VS Visual");
    ofEnableAntiAliasing();
    
    setupOfxMIDIPort();
    
    
    
    gui.setup();

    gui.add(drawLinesToggle.setup("Draw Lines", true));
    gui.add(gridLineColorSelector.setup("Grid Line Color", ofColor::black, ofColor(0, 0), ofColor(255, 255)));
    gui.add(drawBackgroundGridToggle.setup("Draw Rows/Columns", true));
    gui.add(octaveRowColorSelector.setup("Octave Row Color", ofColor(86,0,200,88), ofColor(0, 0), ofColor(255, 255)));
    
    gui.add(noteDisplayColorSelector1.setup("Note Display Color #1", ofColor(0,220,255,255), ofColor(0,0), ofColor(255,255)));
    gui.add(noteDisplayColorSelector2.setup("Note Display Color #2", ofColor(0,5,255,255), ofColor(0,0), ofColor(255,255)));
    
    //ofColor(255,0,83,255)

    
    
    // set global display vars
    windowResized(ofGetWidth(), ofGetHeight());
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    if(drawBackgroundGridToggle)
        drawBgdGrid();
    if(drawLinesToggle)
        drawBgdGridLines();
    
    
    drawActiveNotes();
    
    gui.draw();
}

//--------------------------------------------------------------

void ofApp::drawBgdGridLines(){
    // draw  horizontal grid lines to represent octaves
    ofSetColor(gridLineColorSelector);
    
    for(int i = 0; i < nRows; ++i)
        ofDrawLine(0, i * boxHeight, windowWidth, i * boxHeight);
    
    // draw border column lines
    for(int i = 0; i < nColumns; ++i)
        ofDrawLine(i * boxWidth, 0, i * boxWidth, windowHeight);
}

//--------------------------------------------------------------
void ofApp::drawBgdGrid(){
    // color the even numbered rows with chosen color
    ofSetColor(octaveRowColorSelector);
    for(int i = 0; i < nRows; i += 2)
        ofDrawRectangle(0, i * boxHeight, windowWidth, boxHeight);
    
    // draw black and white background to represent fundamental pitches C - B
    for(int i = 0; i < nColumns; ++i) {
//        // color the even numbered rows with chosen color
//        if((i & 1) == 0){
//            ofSetColor(octaveRowColorSelector);
//            ofDrawRectangle(0, i * boxHeight, windowWidth, boxHeight);
//
//        }
        
        // set color to white for non-accidental keys, black for accidentals
        if(i == 0 || i == 2 || i == 4 || i == 5 || i == 7 || i == 9 || i == 11)
            //if((i > 5 && (i&1)) || ()
            ofSetColor(ofColor::lightGrey, 128);
        else
            ofSetColor(ofColor::darkGrey, 128);
        // draw the column rectangles
        ofDrawRectangle(i * boxWidth, 0, boxWidth, windowHeight);
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
        //ofSetColor(velocity * 2,  255 / std::max(1, noteNumber)/*255 / nColumns * col*/, 255 /*255 / nRows * row*/);
        ofColor noteColor1(noteDisplayColorSelector1), noteColor2(noteDisplayColorSelector2);
        
        //float scaleR = noteColor.get
        
        
        float lerpAmount = velocity * 2.f / 256.f;
        
        
        ofSetColor(noteColor1.getLerped(noteColor2, lerpAmount));
        
        
        ofDrawRectangle(col * boxWidth, row * boxHeight, boxWidth, boxHeight);
        //std::cout<< "Velocity = " << velocity <<", Lerp Amount = " << lerpAmount << '\n';
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
    
    // calculate note box width/height based on window size
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
