#include "NoteGridAnimation.h"

//--------------------------------------------------------------
NoteGridAnimation::NoteGridAnimation(std::string uid) : MIDIAnimationComponent(uid), nColumns(12), nRows(11), boxWidth(), boxHeight(), drawLines(true) {}

//--------------------------------------------------------------
void NoteGridAnimation::setup() {}

//--------------------------------------------------------------
void NoteGridAnimation::setupGUI() {
    // pitch-offset display controls
    gui.add(pitchOffsetSlider.setup("Pitch Offset (Half-Steps)", 0, 0, 11));
    gui.add(pitchOffsetUseMIDICCToggle.setup("Toggle Pitch Offset MIDI CC Control", false));
    pitchOffsetAmount.set("Pitch Offset (Half-Steps) MIDI CC-Controlled Value", 0, 0, 11);
    gui.add(pitchOffsetAmount);
    gui.add(drawLinesToggle.setup("Draw Lines", false));

    gui.add(gridLineColorSelector.setup("Grid Line Color", ofColor::black, ofColor(0, 0), ofColor(255, 255)));
    gui.add(drawBackgroundGridToggle.setup("Draw Rows/Columns", false));
    gui.add(octaveRowColorSelector.setup("Octave Row Color", ofColor(86, 0, 200, 88), ofColor(0, 0), ofColor(255, 255)));

    // 2 colors to interpolate between for default colors
    ofColor color1(0, 5, 255, 255);
    ofColor color2(50, 220, 255, 255);
    int numMIDIChannels = midiPortState->getNumChannels();
    channelColors = new ofxColorSlider[numMIDIChannels];
    for (int i = 0; i < numMIDIChannels; ++i) {
        float lerpAmount = 1.0f / numMIDIChannels * float(i);
        std::string channelName("Channel #");
        channelName.append(std::to_string(i + 1));

        //ofxToggle chanDrawCirclesToggle;
        //chanDrawCirclesToggle.setup(std::string("Draw Circles #").append(std::to_string(i+1)), true);
        //ofxColorSlider chanColor;
        //chanColor.setup(channelName, color2.getLerped(color1, lerpAmount), ofColor(0, 0), ofColor(255, 255));

        ChannelSettings* channelSettings = new ChannelSettings();
        //channelSettings->color.setup(channelName, color2.getLerped(color1, lerpAmount), ofColor(0, 0), ofColor(255, 255));
        channelSettings->drawCirclesToggle.setup(std::string("Draw Circles #").append(std::to_string(i + 1)), true);


        gui.add(&channelSettings->drawCirclesToggle);
        //gui.add(&channelSettings->color);

        channelSettingsList.push_back(channelSettings);

        std::cout << std::boolalpha << channelSettings->drawCirclesToggle;
    }
    
    
    // set global display vars
    windowResized(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void NoteGridAnimation::update() {
    // calculate note box width/height based on window size
    boxWidth = animationWidth / nColumns;
    boxHeight = animationHeight / nRows;

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
        ofDrawLine(0, i * boxHeight, animationWidth, i * boxHeight);

    // draw border column lines
    for (int i = 0; i < nColumns; ++i)
        ofDrawLine(i * boxWidth, 0, i * boxWidth, animationHeight);
}

//--------------------------------------------------------------
void NoteGridAnimation::drawBgdGrid() {
    // color the even numbered rows with chosen color
    ofSetColor(octaveRowColorSelector);
    for (int i = 0; i < nRows; i += 2)
        ofDrawRectangle(0, i * boxHeight, animationWidth, boxHeight);

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
        ofDrawRectangle(i * boxWidth, 0, boxWidth, animationHeight);
    }
}

//--------------------------------------------------------------
void NoteGridAnimation::drawActiveNotes() {
    // draw all notes currently being played
    //for (unsigned int channelNumber = 0; channelNumber < midiPortState->getNumChannels(); ++channelNumber) {
    int channelNum = 0;
    for(auto channel : midiPortState->getAllChannelActiveNoteADSRLevels()){
        /*
        // TODO Look at each midi node instead of by 'held down' notes
        auto ns = midiPortState->getChannelNotes(channelNumber);
        for (int i = 0; i < 128; ++i) {
            auto adsrVal = midiPortState->getADSRValue(channelNumber, i);
            float lerpAmount = velocity * 2.f / 256.f;
            ofSetColor(channelColors[channelNumber], float(velocity) / 128.f  * float(adsrVal) * 256.f );
            ofDrawRectangle(col * boxWidth, row * boxHeight, boxWidth, boxHeight);
        }*/

        // Original method based on notes held down
        //auto ns = midiPortState->getChannelNotes(channelNumber);

        auto channelSettings = channelSettingsList[channelNum];

        
        auto settings = midiPortState->getChannelSettings(channelNum); // TODO combine above line into this call
        for (auto note: channel) {

            //std::cout<< "drawing a note!\n";
            int noteNumber = note.first;//, velocity = note.second;
            int row = nRows - 1 - (noteNumber - pitchOffsetAmount) / 12, col = (noteNumber - pitchOffsetAmount) % nColumns;

            // TODO ensure MIDI NOTE #0 doesn't cause issue (scale midi note #s to start at 1?)
            int velocity = note.second.first;
            float scalarADSR = midiPortState->getADSRValue(channelNum, noteNumber);
            float scalarVelocity = float(velocity) / 128.f;
            float scalar = std::min(scalarADSR * scalarVelocity * 1.1f, 1.f);
            
            ofSetColor(settings->color, scalar * 256.f);
            int startX = col * boxWidth, startY = row * boxHeight;
            
            if (channelSettings->drawCirclesToggle) {
                float radius = float(boxWidth) / 2.f;
                ofDrawCircle({ startX + radius, startY + radius }, radius * scalar); // good 'bloomy', lens-flare circle effect (with purple/red colors)
            }
            else {
                ofDrawRectangle(startX, startY, boxWidth, boxHeight);
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

