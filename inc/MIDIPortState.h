//
//  MIDIPortNotesState.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 8/9/20.
//

#ifndef MIDIPortNotesState_h
#define MIDIPortNotesState_h
#include "ofxMidi.h"
#include "MIDIChannelState.h"
#include <assert.h>

struct MIDIPortStateSettings {
    MIDIPortStateSettings() = default;

    //MIDIPortStateSettings(unsigned int numChannels, bool useVirtualPort, std::string networkPortName, std::string virtualPortName) :
    //    numChannels(numChannels), 
    //    useVirtualPort(useVirtualPort), 
    //    networkPortName(networkPortName), 
    //    virtualPortName(virtualPortName) {

    //}

    //MIDIPortStateSettings& operator= (const MIDIPortStateSettings& state) {
    //    std::cout << " in assignment operator ";
    //    // do the copy
    //    numChannels = state.numChannels;
    //    useVirtualPort = state.useVirtualPort;
    //    networkPortName = state.networkPortName;
    //    virtualPortName = state.virtualPortName;

    //    // return the existing object so we can chain this operator
    //    return *this;
    //}

    // How many midi channels to listen to on the port (1 - 16)
    unsigned int numChannels = 16; 
    // flag set to true if using local MIDI port, false if using network MIDI port
    bool useVirtualPort = false; 
    // names of local and network MIDI ports
    std::string networkPortName = "Network Session 1", virtualPortName = "ofxMidiIn Input";
};



class MIDIPortState : public ofxMidiListener {
public:
    MIDIPortState() = default;


    void setupMIDIPortState(MIDIPortStateSettings settings);

    
   
   
    // TODO allow user to change from virtual port to network port
    // void setUseVirtualPort(bool useVirtualPort){ }
    
    unsigned int getNumChannels();

    
   
    
    /**
     * Gets all the notes currently held down or sustained with pedal on the specified midi channel
     * @param channel - MIDI Channel Number (1-16)
     * TODO Input validation
     */
    std::map<int, int> getChannelNotes(unsigned int channel);

    std::vector<std::map<int, int>> getAllChannelNotes();
    
    const std::map<int, std::pair<int, float>> getAllNotesDown();

    /**
     * Gets the current value of the specified MIDI CC value based on its channel
     * @param channel - MIDI Channel Number (0-15)
     * @param ccNumber - MIDI CC Number (0-127)
     * TODO Input validation
     */
    int getMIDICCValue(unsigned int channel, int ccNumber);
    
    float getADSRValue(unsigned int channel, int noteNumber);
private:
    // Midi Input port
    ofxMidiIn midiIn;

    MIDIPortStateSettings settings;

    // bool initialized; // TODO assert that MIDI port state is initialized in each method
    
    // Array of channel note states represe
    MIDIChannelState* channels;
    
    void validateSettings(const MIDIPortStateSettings& settings);

    void setupMIDIPort();

    /**
     * ofMidiListener implemented method
     */
    void newMidiMessage(ofxMidiMessage& message);
    
};

#endif /* MIDIPortNotesState_h */
