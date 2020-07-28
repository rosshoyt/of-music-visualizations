//
//  MidiNotesState.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 7/27/20.
//

#ifndef MidiNotesState_h
#define MidiNotesState_h
/**
 * Class encapsulating the state of MIDI inputted Note On and Note Offs.
 * Supports concurrent read/write access from multiple threads
 */
class MidiNotesState {
public:
    MidiNotesState() : noteOns(), mtx() {}
    
    void tryAddNoteOn(int note){
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        noteOns.insert(note);
        lck.unlock();
    }
    void tryAddNoteOff(int note){
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        noteOns.erase(note);
        lck.unlock();
    }
    std::vector<int> getNotes(){
        std::vector<int> ret;
        
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        for(auto note: noteOns)
            ret.push_back(note);
        lck.unlock();
        return ret;
    }
private:
    std::set<int> noteOns;
    std::mutex mtx;
};

#endif /* MidiNotesState_h */
