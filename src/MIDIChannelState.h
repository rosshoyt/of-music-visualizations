//
//  MidiNotesState.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 7/27/20.
//

#ifndef MidiNotesState_h
#define MidiNotesState_h
#include "ofxMidi.h"

typedef std::unique_lock<std::mutex> Lock;

class MidiCCNode {
public:
    MidiCCNode() : value(), mtx(){}
    
    int getValue(){
        Lock lck(mtx, std::defer_lock);
        lck.lock();
        int cpy = value;
        lck.unlock();
        return cpy;
    }
    
    int setValue(int value){
        Lock lck(mtx, std::defer_lock);
        lck.lock();
        this->value = value;
        lck.unlock();
    }
    
private:
    int value;
    std::mutex mtx;
};




typedef std::chrono::milliseconds TimeMS;

class Time{
public:
    // TODO move to a Time Utils class
    static TimeMS getCurrentTime(){
        return std::chrono::duration_cast<TimeMS>(std::chrono::system_clock::now().time_since_epoch());
    }
};

// Class which can be shared between NoteADSRState
class ADSR {

public:
    ADSR(int a = 15, int d = 4000, int s = 0, int r = 15) : a(a), d(d), s(s), r(r), aL(1.f), dL(0.f), sL(0.f) {
        total = this->a + this->d + this->r + this->s;
    }
    
    
    float getLevel(TimeMS elapsed){
        float level(0.f);
        
        float start, end, numerator = float(elapsed.count());
        TimeMS divisor;
        if(elapsed < (divisor += a)){
            start = 0.f;
            end = aL;
            //lerpAmt = float(elapsed.count()) / float(a.count());
        } else if(elapsed < (divisor += d)){
            start = dL;
            end = sL;
            
        } else if(elapsed < (divisor += s)){
            start = sL;
            end = 0.f;
        }
        return lerp(start, end, numerator / float(divisor.count()));
        
    }
    
private:
    static float lerp(float a, float b, float f){
        return (a * (1.0 - f)) + (b * f);
    }
    
    TimeMS a, d, s, r, total;
    float aL, dL, sL;
    
    
};

class NoteADSRState{
public:
    
    
    NoteADSRState(ADSR adsr) : adsr(adsr), level(0.f), active(), startTime(), endTime() {
        
    }
    
    
    
    void update(){
        if(active){
            TimeMS elapsed = Time::getCurrentTime() - startTime;
            level = adsr.getLevel(elapsed);
            std::cout<<"Note ADSR Level = " << level << '\n';
        }
    }
    void start(){
        startTime = Time::getCurrentTime();
        active.store(true);
        
    }
    void stop(){
        level = 0.f;
        active.store(false);
    }
    
    float getLevel(){
        return level;
    }
    
    void setADSR(ADSR adsr){
        this->adsr = adsr;
    }
    
private:
    
    std::atomic<bool> active;
    ADSR adsr;
    float level;
    TimeMS startTime, endTime;
};

/**
 * Class encapsulating the state of a single MIDI channel. Processes NoteOn, NoteOff, and all Control Change Messages.
 * Supports concurrent read/write access across multiple threads via mutex locks
 */
class MIDIChannelState {
public:
    MIDIChannelState() : notesHeldDown(), notesSustained(), midiCCState(128), adsrStates(), sustained(false), mtx(), mtxSusNotes() {
        
        ADSR pianoADSR;
        for(int i = 0; i < 128; i++){
            NoteADSRState adsrNode(pianoADSR);
            adsrStates.push_back(&adsrNode);
        }
    }
    
    void processMIDIMessage(ofxMidiMessage& message){
        switch(message.status) {
            case MIDI_NOTE_ON:
                tryAddNoteOn(message.pitch, message.velocity);
                // TODO refactor channel to process each midi message independantly
                break;
                
            case MIDI_NOTE_OFF:
                tryAddNoteOff(message.pitch);
                break;
                
            case MIDI_CONTROL_CHANGE:
                processMIDICC(message);
                
                break;
        }
    }
    
    void processMIDICC(ofxMidiMessage& message){
        switch(message.control){
            case 1:
                tryAddMIDICC(message.control, message.value);
                break;
            case 64:
                switch(message.value){
                    case 0:
                        //std::cout<< "MIDI Control Change # " << message.control << " value = " << message.value << '\n';
                        setSustainPedalOff();
                        break;
                        
                    case 127:
                        
                        setSustainPedalOn();
                        break;
                }
        }
    }
    
    void tryAddNoteOn(int note, int velocity){
        Lock lck(mtx,std::defer_lock);
        lck.lock();
        // insert or update new note/velocity pair
        notesHeldDown[note] = velocity;
        lck.unlock();
        
        adsrStates[note]->start();
    }
    
    void tryAddNoteOff(int note){
        bool clearADSR = true;
        // We may want to continue to hear the note if the sustain pedal is down, so track it
        if(sustained) {
            clearADSR = false;
            
            // Move note to sustainedNotes
            // first get the note's earliest occuring NoteOn Velocity
            // TODO could cause exception if key not exists in the map (cased by corrupted MIDI data)
            int noteOnVel = notesHeldDown.at(note); // 'concurrent access is safe' @s`ee http://www.cplusplus.com/reference/map/map/at/
            // add to sustainedNotes
            Lock lck2 (mtxSusNotes,std::defer_lock);
            lck2.lock();
            // insert or update note value
            notesSustained[note] = noteOnVel;
            lck2.unlock();
            
        } else{
            // If sustain pedal wasn't down, clear the ADSR curve because we will be removing it
            // TODO can add sus pedal tracking to the ADSR class
            adsrStates[note]->stop();
        }
        // Remove note from list of held-down notes
        Lock lck (mtx,std::defer_lock);
        lck.lock();
        notesHeldDown.erase(note);
        lck.unlock();
        
    }
    
    void setSustainPedalOn(){
        sustained.store(true);
    }
    
    void setSustainPedalOff(){
        if(sustained){
            // clear ADSR curves of all the notes that were
            // only being sustained
            for(const auto &nnvp : notesSustained )
                adsrStates[nnvp.first]->stop();
            
            
            // clear all notes from sustainedNotes
            Lock lck (mtxSusNotes,std::defer_lock);
            lck.lock();
            notesSustained.clear();
            lck.unlock();
            
            sustained.store(false);
            
        }
    }
    // TODO input validation
    float getADSRLevel(int note){
        return adsrStates[note]->getLevel();
    }
    std::map<int,int> getNotes(){
        Lock lck (mtx,std::defer_lock);
        lck.lock();
        // copy note map
        auto ret = notesHeldDown;
        lck.unlock();
        return ret;
    }
    std::map<int,int> getSustainedNotes(){
        Lock lck (mtxSusNotes,std::defer_lock);
        lck.lock();
        // copy sus notes map
        auto ret = notesSustained;
        lck.unlock();
        return ret;
    }
    
    std::map<int,int> getAllNotes(){
        auto notes = getNotes();
        auto susNotes = getSustainedNotes();
        notes.insert(susNotes.begin(), susNotes.end());
        return notes;
    }
    
    int getNumNotes(){
        Lock lck (mtx,std::defer_lock);
        lck.lock();
        int n = notesHeldDown.size();
        lck.unlock();
        return n;
    }
    
    void tryAddMIDICC(int midiCC, int value) {
        //std::cout << "Recieved midi CC #1 val = " << value << '\n';
        Lock lck (mtx,std::defer_lock);
        lck.lock();
        midiCCState[midiCC].setValue(value);
        lck.unlock();
    }
    
    int tryGetCCValue(int ccNo){
        Lock lck (mtx,std::defer_lock);
        lck.lock();
        int cpy = midiCCState[ccNo].getValue();
        lck.unlock();
        //std::cout << "returning cc#" << ccNo << " value = " << cpy << '\n';
        return cpy;
    }
    
    
    bool sustainPedalIsDown(){
        return sustained.load();
    }
    
private:
    
    std::map<int,int> notesHeldDown;
    std::map<int,int> notesSustained;
    std::vector<NoteADSRState*> adsrStates;
    
    std::atomic<bool>  sustained;
    
    std::mutex mtx;
    std::mutex mtxSusNotes;
    
    std::vector<MidiCCNode> midiCCState;
    
    
    
};

#endif /* MidiNotesState_h */
