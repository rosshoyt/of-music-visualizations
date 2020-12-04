//
//  MidiNotesState.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 7/27/20.
//

#ifndef MidiNotesState_h
#define MidiNotesState_h
#include "ofxMidi.h"
#include "ofxGui.h"
#include "Envelope.h"

class MIDIChannelSettings {
public:
    // gui params
    ofParameter<ofColor> color;
    Envelope volumeEnvelope;

    //ofxGuiGroup params; // TODO
    ofParameterGroup params; // TODO

    //std::vector<ofAbstractParameter> params;

    MIDIChannelSettings() { 
        // TODO add channel number to the gui names
        params.add(color.set("Note Color", ofColor::cornflowerBlue));
        params.add(volumeEnvelope.guiParams);
        
        //params.minimize();
        //volumeEnvelope.guiParams.setName()
    }

   /* int getChannelNum() {
        return channelNum;
    }*/
private:
    // TODO implement
    //ShapeType shape = TRIANGLE;
    //ofParameter<int> pitchOffsetAmount;
    //ADSR adsr;

    //int channelNum = 0;

};

typedef std::unique_lock<std::mutex> Lock;

class MidiCCNode {
public:
    MidiCCNode();
    
    int getValue();
    
    void setValue(int value);
    
private:
    int value;
    std::mutex mtx;
};

/**
 * Class encapsulating the state of a single MIDI channel. Processes NoteOn, NoteOff, and all Control Change Messages.
 * Supports concurrent read/write access across multiple threads via mutex locks
 */
class MIDIChannelState {
public:
    MIDIChannelState();
    
    void processMIDIMessage(ofxMidiMessage& message);
    
    void processMIDICC(ofxMidiMessage& message);
    
    void tryAddNoteOn(int note, int velocity);
    
    void tryAddNoteOff(int note);
    
    void setSustainPedalOn();
    
    void setSustainPedalOff();
    
    // TODO input validation
    float getADSRLevel(int note);
    
    std::map<int, int> getNotes();
    std::map<int, int> getSustainedNotes();
    
    std::map<int, int> getAllNotes();

    std::map<int, std::pair<int, float>> getAllActiveNoteADSRLevels() {
        std::map<int, std::pair<int, float>> ret;
        for (int i = 0; i < 128; ++i) {
            auto level = adsrStates[i]->getLevel();
            auto velocity = adsrStates[i]->getLastNoteOnVelocity();
            // TODO ensure no probelm caused by notes being numbered 0 - 127
            if (level > 0) 
                ret.insert({ i, {  velocity , level } });
        }
        return ret;
    }
    
    int getNumNotes();
    
    MIDIChannelSettings* getChannelSettings() {
        return settings;
    }

    void tryAddMIDICC(int midiCC, int value);
    
    int tryGetCCValue(int ccNo);
    
    
    bool sustainPedalIsDown();

    
private:
    MIDIChannelSettings* settings;

    std::map<int,int> notesHeldDown;
    std::map<int,int> notesSustained;


    // TODO rename (might not be ADSR, could be ADR)
    std::vector<EnvelopeNode*> adsrStates;
    std::atomic<bool>  sustained;
    
    std::mutex mtx;
    std::mutex mtxSusNotes;
    
    std::vector<MidiCCNode> midiCCState;
    
    unsigned int channelNumber;
    
};

#endif /* MidiNotesState_h */
