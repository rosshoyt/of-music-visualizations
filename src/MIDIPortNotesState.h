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
        //for(int i = 0; i < numChannels; ++i)
        //    channels[i] = new MIDIChannelNotesState();
    }
    
    std::map<int,int> getChannelNotes(unsigned int channel){
        return channels[channel].getNotes();
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
                     //std::cout << "Setting pitch #" << message.pitch << " on, velocity = " << message.velocity  << "\n";
                    channels[channel].tryAddNoteOn(message.pitch, message.velocity);
                    break;
                case MIDI_NOTE_OFF:
                    //std::cout << "Setting pitch #" << message.pitch << " off\n";
                    channels[channel].tryAddNoteOff(message.pitch);
                    break;
                case MIDI_CONTROL_CHANGE:
                    // process MIDI Control Changes
                    switch(message.control){
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
                    }
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
