//
//  MIDIPortNotesState.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 8/9/20.
//

#ifndef MIDIPortNotesState_h
#define MIDIPortNotesState_h

#import "MIDIChannelState.h"

class MIDIPortNotesState : public ofxMidiListener {
public:
    MIDIPortNotesState(std::string portName, bool useVirtualPort, unsigned int numChannels = 16) : portName(portName), useVirtualPort(useVirtualPort), numChannels(numChannels > 16 ? 16 : numChannels){
        
        setupOfxMIDIPort();
        channels = new MIDIChannelState[this->numChannels];
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
     * @param channel - MIDI Channel Number (1-16)
     * @param ccNumber - MIDI CC Number (0-127)
     * TODO Input validation
     */
    int getMIDICCValue(unsigned int channel, int ccNumber){
        return channels[channel - 1].tryGetCCValue(ccNumber);
    }
private:
    // Midi Input port
    ofxMidiIn midiIn;
    // flag set to true if using local MIDI port, false if using network MIDI port
    bool useVirtualPort;
    // names of local and network MIDI ports.
    std::string portName;
    
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
    
    void setupOfxMIDIPort(){
        if(useVirtualPort) {
            std::cout << "Setting up local virtual midi port " << portName << "\n";
            midiIn.openVirtualPort();
            std::cout<< portName <<" port # is "<< midiIn.getPort() << '\n';
        }
        else {
            for(std::string portIn : midiIn.getInPortList()) {
                std::cout << "Available InPort: " << portIn << '\n';
                if(portIn.compare(portName) == 0){
                    midiIn.openPort(portName);
                    if(midiIn.isOpen())
                        std::cout << "Found desired network port and opened it \n";
                }
            }
            
        }
        midiIn.addListener(this);
    }
    
    
};

#endif /* MIDIPortNotesState_h */
