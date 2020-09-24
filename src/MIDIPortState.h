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

class MIDIPortState : public ofxMidiListener {
public:
    MIDIPortState(unsigned int numChannels = 16, bool useVirtualPort = true, std::string networkPortName = "Network Session 1", std::string virtualPortName = "ofxMidiIn Input") : useVirtualPort(useVirtualPort), networkPortName(networkPortName), virtualPortName(virtualPortName), numChannels(numChannels) {
        // set the number of channels with input validation, and initialize channel state map
        setNumChannels(numChannels);
    }
    
    void setNumChannels(unsigned int numChannels){
        // ensure numChannels is between 1 and 16 (inclusive)
        if(numChannels > 16)
            this->numChannels = 16;
        else if(numChannels == 0)
            this->numChannels = 1;
        // initialize channel state map TODO move to separate function and only re-initialize when needed
        channels = new MIDIChannelState[this->numChannels];
    }

    // TODO allow user to change from virtual port to network port
    // void setUseVirtualPort(bool useVirtualPort){ }
    
    unsigned int getNumChannels(){
        return this->numChannels;
    }

    
    void setupMIDIPort(){
        if(useVirtualPort) {
            std::cout << "Setting up local virtual midi port " << virtualPortName << "\n";
            midiIn.openVirtualPort();
            std::cout<< virtualPortName << " port # is "<< midiIn.getPort() << '\n';
        }
        else {
            for(std::string portIn : midiIn.getInPortList()) {
                std::cout << "Available InPort: " << portIn << '\n';
                if(portIn.compare(networkPortName) == 0){
                    midiIn.openPort(networkPortName);
                    if(midiIn.isOpen())
                        std::cout << "Found desired network port and opened it \n";
                }
            }
            
        }
        midiIn.addListener(this);
    }
    
    /**
     * Gets all the notes currently held down or sustained with pedal on the specified midi channel
     * @param channel - MIDI Channel Number (1-16)
     * TODO Input validation
     */
    std::map<int,int> getChannelNotes(unsigned int channel){
        return channels[channel].getAllNotes();
    }
    
    /**
     * Gets the current value of the specified MIDI CC value based on its channel
     * @param channel - MIDI Channel Number (0-15)
     * @param ccNumber - MIDI CC Number (0-127)
     * TODO Input validation
     */
    int getMIDICCValue(unsigned int channel, int ccNumber){
        return channels[channel].tryGetCCValue(ccNumber);
    }
    
    float getADSRValue(unsigned int channel, int noteNumber){
        return channels[channel].getADSRLevel(noteNumber);
    }
private:
    // Midi Input port
    ofxMidiIn midiIn;
    // flag set to true if using local MIDI port, false if using network MIDI port
    bool useVirtualPort;
    // names of local and network MIDI ports.
    std::string networkPortName, virtualPortName;
    
    // How many midi channels to listen to on the port (1 - 16)
    unsigned int numChannels;
    
    // Array of channel note states represe
    MIDIChannelState* channels;
    
    /**
     * ofMidiListener implemented method
     */
    void newMidiMessage(ofxMidiMessage& message){
        unsigned int channel(message.channel - 1);
        if(channel < numChannels){
            channels[channel].processMIDIMessage(message);
        }
    }
    
};

#endif /* MIDIPortNotesState_h */
