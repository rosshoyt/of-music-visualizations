//
//  MidiNotesState.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 7/27/20.
//

#ifndef MidiNotesState_h
#define MidiNotesState_h

#include "Note.h"
/**
 * Class encapsulating the state of MIDI inputted Note On and Note Offs.
 * Supports concurrent read/write access from multiple threads
 */
class MidiNotesState {
public:
    MidiNotesState() : noteOns(), mtx() {}
    
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
    std::map<int,int> getNotes(){
        //std::vector<int> ret;
        std::unique_lock<std::mutex> lck (mtx,std::defer_lock);
        lck.lock();
        // copy map
        auto ret = noteOns;
        lck.unlock();
        return ret;
    }
private:
    
    std::map<int, int> noteOns;
    std::mutex mtx;



};

#endif /* MidiNotesState_h */
