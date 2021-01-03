#include "NoteGridAnimation.h"

//--------------------------------------------------------------
NoteGridAnimation::NoteGridAnimation(std::string uid) : MIDIAnimationGUIComponent(uid), nColumns(12), nRows(11), boxWidth(), boxHeight(), drawLines(true) {}

//--------------------------------------------------------------
void NoteGridAnimation::setup() {}

//--------------------------------------------------------------
void NoteGridAnimation::setupGUI() {
    // pitch-offset display controls
    gui.add(pitchOffsetSlider.setup("Pitch Offset (Half-Steps)", 0, 0, 11));
    gui.add(pitchOffsetUseMIDICCToggle.setup("Toggle Pitch Offset MIDI CC Control", false));
    pitchOffsetAmount.set("Pitch Offset (Half-Steps) MIDI CC-Controlled Value", 0, 0, 11);
    gui.add(pitchOffsetAmount);
    gui.add(noteSizeScale.set("Size", 1, .1, 10));
    gui.add(drawLinesToggle.setup("Draw Lines", false));

    gui.add(gridLineColorSelector.setup("Grid Line Color", ofColor::black, ofColor(0, 0), ofColor(255, 255)));
    gui.add(drawBackgroundGridToggle.setup("Draw Rows/Columns", false));
    gui.add(octaveRowColorSelector.set("Octave Row Color", ofColor(86, 0, 200, 88), ofColor(0, 0), ofColor(255, 255)));


    // 2 colors to interpolate between for default colors
    ofColor color1(0, 5, 255, 255);
    ofColor color2(50, 220, 255, 255);
    int numMIDIChannels = midiPortState->getNumChannels();
    channelColors = new ofxColorSlider[numMIDIChannels];
    for (int i = 0; i < numMIDIChannels; ++i) {
        float lerpAmount = 1.0f / numMIDIChannels * float(i);
        std::string channelName("Channel #");
        channelName.append(std::to_string(i + 1));

        ChannelSettings* channelSettings = new ChannelSettings();
        channelSettings->drawCirclesToggle.setup(std::string("Draw Circles #").append(std::to_string(i + 1)), true);


        gui.add(&channelSettings->drawCirclesToggle);

        channelSettingsList.push_back(channelSettings);
    }
    
    
    // set global display vars
    //windowResized(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void NoteGridAnimation::setupParameterListeners() {
    addParameterListener(octaveRowColorSelector);
}

//--------------------------------------------------------------
void NoteGridAnimation::update() {
    // calculate note box width/height based on window size
    boxWidth = AnimationComponent::getAnimationWidth() / nColumns;
    boxHeight = AnimationComponent::getAnimationHeight() / nRows;

    // update the pitch offset amount
    if (pitchOffsetUseMIDICCToggle) 
        pitchOffsetAmount.set(std::round(midiPortState->getMIDICCValue(1, 1) / 12.f));
    else
        pitchOffsetAmount.set(pitchOffsetSlider);
}

//--------------------------------------------------------------
void NoteGridAnimation::draw() {
    //ofBackground(backgroundColorSelector);
    //drawGUI(); // TODO move outside of class

    if (drawBackgroundGridToggle)
        drawBgdGrid();
    if (drawLinesToggle)
        drawBgdGridLines();

    drawActiveNotes();    
}

//--------------------------------------------------------------
void NoteGridAnimation::drawBgdGridLines() {
    // draw  horizontal grid lines to represent octaves
    ofSetColor(gridLineColorSelector);

    for (int i = 0; i < nRows; ++i)
        ofDrawLine(0, i * boxHeight, AnimationComponent::getAnimationWidth(), i * boxHeight);

    // draw border column lines
    for (int i = 0; i < nColumns; ++i)
        ofDrawLine(i * boxWidth, 0, i * boxWidth, AnimationComponent::getAnimationHeight());
}

//--------------------------------------------------------------
void NoteGridAnimation::drawBgdGrid() {
    // color the even numbered rows with chosen color
    ofSetColor(octaveRowColorSelector);
    for (int i = 0; i < nRows; i += 2)
        ofDrawRectangle(0, i * boxHeight, AnimationComponent::getAnimationWidth(), boxHeight);

    // draw black and white background to represent fundamental pitches C - B
    for (int i = 0; i < nColumns; ++i) {
        int colPitch = (i + pitchOffsetAmount) % nColumns;
        // set color to white for non-accidental keys, black for accidentals
        if (colPitch == 0 || colPitch == 2 || colPitch == 4 || colPitch == 5 || colPitch == 7 || colPitch == 9 || colPitch == 11)
            //if((i > 5 && (i&1)) || ()
            ofSetColor(ofColor::lightGrey, 128);
        else
            ofSetColor(ofColor::darkGrey, 128);
        // draw the column rectangles
        ofDrawRectangle(i * boxWidth, 0, boxWidth, AnimationComponent::getAnimationHeight());
    }
}

//--------------------------------------------------------------
void NoteGridAnimation::drawActiveNotes() {
    // draw all notes currently being played
    //for (unsigned int channelNumber = 0; channelNumber < midiPortState->getNumChannels(); ++channelNumber) {
    int channelNum = 0;
    for(auto channel : midiPortState->getAllChannelActiveNoteADSRLevels()){
      
        auto animationSpecificChannelSettings = channelSettingsList[channelNum];

        auto settings = midiPortState->getChannelSettings(channelNum); // TODO combine above line into this call
        for (auto note: channel) {

           
            int noteNumber = note.first;//, velocity = note.second;

            int row = nRows - 1 - (noteNumber - pitchOffsetAmount) / 12;
            int col = (noteNumber - pitchOffsetAmount) % nColumns;

            int velocity = note.second.first;
            float scalarADSR = midiPortState->getADSRValue(channelNum, noteNumber);
            float scalarVelocity = float(velocity) / 128.f;
            float scalar = std::min(scalarADSR * scalarVelocity * 1.1f, 1.f) * settings->size;
            

            ofSetColor(settings->color, scalarADSR * scalarVelocity * 256.f);


            int startX = col * boxWidth, startY = row * boxHeight;
            
            if (animationSpecificChannelSettings->drawCirclesToggle) {
                float radius = float(boxWidth) / 2.f;
                ofDrawCircle({ startX + radius, startY + radius }, radius * scalar); 
            }
            else {
                ofDrawRectangle(startX, startY, boxWidth * scalar, boxHeight * scalar);
            }

            // debug msgs TODO delete
            //std::cout << "Note #" << noteNumber << " ADSR val = " << adsrVal << '\n';
            //std::cout<< "Velocity = " << velocity <<", Lerp Amount = " << lerpAmount << '\n';
            //std::cout << "col: " << col << ", row: " << row << '\n';
           
        }
        ++channelNum;
    }
}

//--------------------------------------------------------------
void NoteGridAnimation::windowResized(int w, int h) {}

