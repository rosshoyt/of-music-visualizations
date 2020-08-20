//
//  MIDIPortNotesState.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 8/9/20.
//

#ifndef MIDIPortNotesState_h
#define MIDIPortNotesState_h

#import "MIDIChannelNotesState.h"

class MIDIPortNotesState : public ofxMidiListener {
public:
    MIDIPortNotesState(std::string portName, bool useVirtualPort, unsigned int numChannels) : portName(portName), useVirtualPort(useVirtualPort), numChannels(numChannels > 16 ? 16 : numChannels){
        
        setupOfxMIDIPort();
        channels = new MIDIChannelNotesState[this->numChannels];
    }
    
    /**
     * Gets all the notes currently held down or sustained with pedal on the specified midi channel
     * @param channel - MIDI Channel Number (1-16)
     * TODO Input validation
     */
    std::map<int,int> getChannelNotes(unsigned int channel){
        return channels[channel].getNotes();
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
    MIDIChannelNotesState* channels;
    
    /**
     * ofMidiListener implemented method
     */
    void newMidiMessage(ofxMidiMessage& message){
        unsigned int channel(message.channel - 1);
        if(channel < numChannels){
            switch(message.status) {
                case MIDI_NOTE_ON:
                    channels[channel].tryAddNoteOn(message.pitch, message.velocity);
                    // TODO refactor channel to process each midi message independantly
                    break;
                    
                case MIDI_NOTE_OFF:
                    channels[channel].tryAddNoteOff(message.pitch);
                    break;
                    
                case MIDI_CONTROL_CHANGE:
                    // process MIDI Control Changes
                    switch(message.control){
                        case 1:
                            // TODO eliminate data redundancy between classes (move CC handling to MidiChannelNotesState.h
                             channels[channel].tryAddMIDICC(message.control, message.value);
                            break;
                            
                        case 64:
                            switch(message.value){
                                case 0:
                                    //std::cout<< "MIDI Control Change # " << message.control << " value = " << message.value << '\n';
                                    channels[channel].setSustainPedalOff();
                                    break;
                                    
                                case 127:
                                    //std::cout<< "MIDI Control Change # " << message.control << " value = " << message.value << '\n';
                                    channels[channel].setSustainPedalOn();
                                    break;
                            }
                            channels[channel].tryAddMIDICC(message.control, message.value);
                    }
                    break;
            }
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
