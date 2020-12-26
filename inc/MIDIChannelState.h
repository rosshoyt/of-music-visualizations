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
#include "GUIParameterNode.h"

class MIDIChannelSettings {
public:
    // gui params
    ofParameter<ofColor> color;
    Envelope volumeEnvelope;

    ofParameterGroup params;

    MIDIChannelSettings() {
        init();
    }

    MIDIChannelSettings(Envelope volumeEnvelope) : volumeEnvelope(volumeEnvelope) {
        init();
    }

private:
    void init() {
        // TODO add channel number to the gui names
        params.add(color.set("Note Color", ofColor::cornflowerBlue));

        params.add(volumeEnvelope.guiParams);
        std::cout << "Created MIDIChannel Settings with volume envelope controls containing " << volumeEnvelope.guiParams.size() << " items \n";
    }

    // todo
    //int channelNum;
};

typedef std::unique_lock<std::mutex> Lock;

class MidiCCNode {
public:
    MidiCCNode();
    
    int getValue();
    
    void setValue(int value);

    void reset();

    void setParameterNode(GUIParameterNode* node);

    // TODO: void removeParameterMappings();
    
private:
    int value;

    std::mutex mtx;

    std::vector<GUIParameterNode*> params;

    static const int MAX_CC_VAL = 127;
};

/**
 * Class encapsulating the state of a single MIDI channel. Processes NoteOn, NoteOff, and all Control Change Messages.
 * Supports concurrent read/write access across multiple threads via mutex locks
 */
class MIDIChannelState {
public:
    MIDIChannelState(MIDIChannelSettings* settings);

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

    std::map<int, std::pair<int, float>> getAllActiveNoteADSRLevels();
    
    int getNumNotes();
    
    MIDIChannelSettings* getChannelSettings();

    void tryAddMIDICC(int midiCC, int value);
    
    int tryGetCCValue(int ccNo);
    
    bool sustainPedalIsDown();

    void resetNotes();

    void addCCControlledParam(GUIParameterNode* node, int ccNum);

private:
    MIDIChannelSettings* settings;
    
    std::map<int,int> notesHeldDown;
    std::map<int,int> notesSustained;

    // list of each note's envelope state
    std::vector<EnvelopeNode*> envelopeStates;
    // list of each MIDI CC State, also stores MIDI CC controlled GUI parameters
    std::vector<MidiCCNode> midiCCState;

    // flag storing if the sustain pedal is down
    std::atomic<bool> sustained;
    
    // mutexs used to synchronize access to data structures across multiple threads
    std::mutex mtx;
    std::mutex mtxSusNotes;
   
    // the MIDI channel number 
    unsigned int channelNumber;
};

#endif /* MidiNotesState_h */
