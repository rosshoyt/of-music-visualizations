//
//  MidiNotesState.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 7/27/20.
//

#ifndef MidiNotesState_h
#define MidiNotesState_h
#include "ofxMidi.h"
#include "ADSR.h"


class Settings {
public:
    ofParameter<ofColor> color;

    ofParameterGroup params;
    //std::vector<ofAbstractParameter> params;

    Settings() { 
        params.add(color.set(ofColor::cornflowerBlue));
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
    
    int getNumNotes();
    
    Settings* getChannelSettings() {
        return settings;
    }

    void tryAddMIDICC(int midiCC, int value);
    
    int tryGetCCValue(int ccNo);
    
    
    bool sustainPedalIsDown();

    
private:
    Settings* settings;

    std::map<int,int> notesHeldDown;
    std::map<int,int> notesSustained;


    std::vector<NoteADSRState*> adsrStates;
    
    std::atomic<bool>  sustained;
    
    std::mutex mtx;
    std::mutex mtxSusNotes;
    
    std::vector<MidiCCNode> midiCCState;
    
    unsigned int channelNumber;
    
};

#endif /* MidiNotesState_h */
