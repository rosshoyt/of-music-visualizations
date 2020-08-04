//
//  MidiNotesState.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 7/27/20.
//

#ifndef MidiNotesState_h
#define MidiNotesState_h


/**
 * Class encapsulating the state of MIDI inputted Note On and Note Offs for a single MIDI channel.
 * Supports concurrent read/write access from multiple threads.
 */
class MIDIChannelNotesState {
public:
    MIDIChannelNotesState() : activeNoteOns(), sustainedNotes(), sustained(false), mtx(), mtxSusNotes() {}
    
    
    void tryAddNoteOn(int note, int velocity){
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        activeNoteOns.insert( { note, velocity } );
        lck.unlock();
    }
void tryAddNoteOff(int note){
    
    if(sustained) {
        std::cout << "Sustain pedal down, copying note to sustainedNotes map\n";
        // we'll need to move note from activeNoteOns to sustainedNotes
        
        // first track original note on's velocity
        int noteOnVel = activeNoteOns.at(note); // 'concurrent access is safe' @see http://www.cplusplus.com/reference/map/map/at/
        
        // remove note from activeNoteOnsMap
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        activeNoteOns.erase(note);
        lck.unlock();
        
        // add to sustainedNotes
        std::unique_lock<std::mutex> lck2 (mtxSusNotes,std::defer_lock);
        lck2.lock();
        sustainedNotes.insert({ note, noteOnVel });
        lck2.unlock();
        
        std::cout << "Num sustained notes = " << sustainedNotes.size() <<"\n";
        
    }
    else{
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        activeNoteOns.erase(note);
        lck.unlock();
    }
    
    }
    
    void setSustainPedalOn(){
        std::cout << "Sus Pedal On\n";
        sustained.store(true);
    }
    
    void setSustainPedalOff(){
        
        std::cout << "Sus Pedal Off\n";
        //std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        //lck.lock();
        if(sustained){
            sustained.store(false);
            
            // clear all notes from sustainedNotes
            std::unique_lock<std::mutex> lck (mtxSusNotes,std::defer_lock);
            lck.lock();
            sustainedNotes.clear();
            lck.unlock();
            
        }
        
        //lck.unlock();
    }
    
    std::map<int,int> getNotes(){
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        // copy note map
        auto ret = activeNoteOns;
        lck.unlock();
        return ret;
    }
    std::map<int,int> getSustainedNotes(){
        std::unique_lock<std::mutex> lck (mtxSusNotes,std::defer_lock);
        lck.lock();
        // copy sus notes map
        auto ret = sustainedNotes;
        lck.unlock();
        return ret;
    }
    
    std::map<int,int> getAllNotes(){
        auto notes = getNotes();
        auto susNotes = getSustainedNotes();
        notes.insert(susNotes.begin(), susNotes.end());
        return notes;
//        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
//        lck.lock();
//        auto
    }
    
    int getNumNotes(){
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        int n = activeNoteOns.size();
        lck.unlock();
        return n;
    }
    
    bool sustainPedalIsDown(){
        return sustained.load();
    }
    
private:
    
    std::map<int, int> activeNoteOns;
    std::map<int,int> sustainedNotes;
    
    std::atomic<bool>  sustained;
    
    std::mutex mtx;
    std::mutex mtxSusNotes;
    


};

#endif /* MidiNotesState_h */
