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
#include "GUISubComponent.h"

class MIDIChannelSettings : public GUISubComponent{
public:
    // gui params
    ofParameter<std::string> textNotesBox;
    ofParameter<ofColor> color = ofColor::cornflowerBlue;
    ofParameter<float> size = 1.f;
    Envelope volumeEnvelope;

    MIDIChannelSettings(GUIComponent* par) : GUISubComponent(par) {
       
    }

    MIDIChannelSettings(GUIComponent* par, Envelope volumeEnvelope) : GUISubComponent(par), volumeEnvelope(volumeEnvelope) {
        
    }

    // Inherited via GUISubComponent
    void setupParamGroup() override {
        // TODO add channel number to the gui names
        paramGroup.add(textNotesBox.set("[Name/Track Info]"));
        paramGroup.add(color.set("Note Color", color));
        paramGroup.add(size.set("Size", size, .5, 8));
        paramGroup.add(volumeEnvelope.guiParams);
        //std::cout << "Created MIDIChannel Settings with volume envelope controls containing " << volumeEnvelope.guiParams.size() << " items \n";
        
        parent->addParameterListener(color);
        parent->addParameterListener(size);
    }

    void setupParameterListeners() override {
        
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
