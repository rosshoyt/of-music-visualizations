#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : midiPortState(4, false), windowWidth(), windowHeight(), nColumns(12), nRows(11), boxWidth(), boxHeight(), drawLines(true), backgroundColor() {

}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowTitle("VS Visual");
    ofEnableAntiAliasing();
    
    gui.setup();
    // pitch-offset display controls
    gui.add(pitchOffsetSlider.setup("Pitch Offset (Half-Steps)", 0, 0, 11));
    gui.add(pitchOffsetUseMIDICCToggle.setup("Toggle Pitch Offset MIDI CC Control", false));
    pitchOffsetAmount.set("Pitch Offset (Half-Steps) MIDI CC-Controlled Value",0,0,11);
    gui.add(pitchOffsetAmount);
    
    // background/animation view controls
    gui.add(drawLinesToggle.setup("Draw Lines", true));
    gui.add(gridLineColorSelector.setup("Grid Line Color", ofColor::black, ofColor(0, 0), ofColor(255, 255)));
    gui.add(drawBackgroundGridToggle.setup("Draw Rows/Columns", true));
    gui.add(octaveRowColorSelector.setup("Octave Row Color", ofColor(86,0,200,88), ofColor(0, 0), ofColor(255, 255)));
    
    // 2 colors to interpolate between for default colors
    ofColor color1(0,5,255,255);
    ofColor color2(50,220,255,255);
    
    int numMIDIChannels = midiPortState.getNumChannels();
    channelColors = new ofxColorSlider[numMIDIChannels];
    for(int i = 0; i < numMIDIChannels; ++i){
        std::string channelName("Channel Color #");
        channelName.append(std::to_string(i+1));
        float lerpAmount = 1.0f / numMIDIChannels * float(i) ;
        gui.add(channelColors[i].setup(channelName, color2.getLerped(color1, lerpAmount), ofColor(0,0), ofColor(255,255)));
    }
    
    // initialize network MIDI port
    midiPortState.setupMIDIPort();
    
    // set global display vars
    windowResized(ofGetWidth(), ofGetHeight());
    
}

//--------------------------------------------------------------
void ofApp::update(){
    // update the pitch offset amount
    if(pitchOffsetUseMIDICCToggle) pitchOffsetAmount.set(std::round(midiPortState.getMIDICCValue(1, 1) / 12.f));
    else
        pitchOffsetAmount.set(pitchOffsetSlider);
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
        int colPitch = (i + pitchOffsetAmount) % nColumns;
        // set color to white for non-accidental keys, black for accidentals
        if(colPitch == 0 || colPitch == 2 || colPitch == 4 || colPitch == 5 || colPitch == 7 || colPitch == 9 || colPitch == 11)
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
    for(unsigned int channelNumber = 0; channelNumber < midiPortState.getNumChannels(); ++channelNumber) {
        
        auto ns = midiPortState.getChannelNotes(channelNumber);
        for(auto note : ns) {
            //std::cout<< "drawing a note!\n";
            int noteNumber = note.first, velocity = note.second;
            int row = nRows - 1 - (noteNumber - pitchOffsetAmount) / 12, col = (noteNumber - pitchOffsetAmount)  % nColumns;
            
            // TODO ensure MIDI NOTE #0 doesn't cause issue (scale midi note #s to start at 1?)
            //ofSetColor(velocity * 2,  255 / std::max(1, noteNumber)/*255 / nColumns * col*/, 255 /*255 / nRows * row*/);
            //ofColor noteColor1(noteDisplayColorSelector1), noteColor2(noteDisplayColorSelector2);
            //float scaleR = noteColor.get
            
            float lerpAmount = velocity * 2.f / 256.f;
            ofSetColor(channelColors[channelNumber],velocity * 2 );
            ofDrawRectangle(col * boxWidth, row * boxHeight, boxWidth, boxHeight);
            
            // debug msgs TODO delete
            auto adsrVal = midiPortState.getADSRValue(channelNumber, noteNumber);
            //std::cout << "Note #" << noteNumber << " ADSR val = " << adsrVal << '\n';
            //std::cout<< "Velocity = " << velocity <<", Lerp Amount = " << lerpAmount << '\n';
            //std::cout << "col: " << col << ", row: " << row << '\n';
        }
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
