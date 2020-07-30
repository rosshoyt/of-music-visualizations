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
    MIDIChannelNotesState() : noteOns(), sustainedNotes(), sustainPedalDown(false), mtx() {}
    
    
    void tryAddNoteOn(int note, int velocity){
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        noteOns.insert( { note, velocity } );
        lck.unlock();
    }
void tryAddNoteOff(int note){
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        noteOns.erase(note);
        lck.unlock();
    }
    
    void trySetSustainPedalOn(){
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        sustainPedalDown = true;
        lck.unlock();
    }
    
    void trySetSustainPedalOff(){
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        sustainPedalDown = false;
        lck.unlock();
    }
    
    std::map<int,int> getNotes(){
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        // copy note map
        auto ret = noteOns;
        lck.unlock();
        return ret;
    }
private:
    
    std::map<int, int> noteOns;
    std::map<int, int> sustainedNotes;
    std::atomic<bool> sustainPedalDown;
    
    std::mutex mtx;
    


};

#endif /* MidiNotesState_h */
