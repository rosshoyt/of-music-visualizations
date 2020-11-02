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


    void setupMIDIPortState(MIDIPortStateSettings settings) {
        validateSettings(settings);
        this->settings = settings;
        std::cout << this->settings.numChannels << " = num Channels!!!!!!\n";
        // initialize channel state map TODO move to separate function and only re-initialize when needed
        channels = new MIDIChannelState[this->settings.numChannels];
        setupMIDIPort();
    }

    
   
   
    // TODO allow user to change from virtual port to network port
    // void setUseVirtualPort(bool useVirtualPort){ }
    
    unsigned int getNumChannels(){
        return settings.numChannels;
    }

    
   
    
    /**
     * Gets all the notes currently held down or sustained with pedal on the specified midi channel
     * @param channel - MIDI Channel Number (1-16)
     * TODO Input validation
     */
    std::map<int,int> getChannelNotes(unsigned int channel){
        return channels[channel].getAllNotes();
    }

    std::vector<std::map<int, int>> getAllChannelNotes() {
        std::vector<std::map<int,int>> channelNotesList;
        for (unsigned int i = 0; i < settings.numChannels; ++i) {
            channelNotesList.push_back(getChannelNotes(i));
        }
        return channelNotesList;
    }
    
    const std::map<int, std::pair<int, float>> getAllNotesDown() {
        // Get the current active MIDI notes for all channels and
        // store in map<midiPitch, <velocity, adsr value>>
        std::map<int, std::pair<int, float>> allNotesDown;
        int channelNum = 0;
        for (auto channelNotes : getAllChannelNotes()) {
            for (auto note : channelNotes) {
                // TODO don't overwrite values when 2 notes are same between channels
                allNotesDown.insert({ note.first, { note.second, getADSRValue(channelNum, note.first) } });
            }
            ++channelNum;
        }
        return allNotesDown;
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

    MIDIPortStateSettings settings;

    // bool initialized; // TODO assert that MIDI port state is initialized in each method
    
    // Array of channel note states represe
    MIDIChannelState* channels;
    
    void validateSettings(const MIDIPortStateSettings& settings) {
        std::cout << settings.numChannels << " num Channels";
        // ensure numChannels is between 1 and 16 (inclusive)
        assert(this->settings.numChannels > 0 && this->settings.numChannels >= 16);
       
        
        //if (this->settings.numChannels > 16) this->settings.numChannels = 16;
        //else if (this->settings.numChannels == 0) this->settings.numChannels = 1;

    }

    void setupMIDIPort() {
        if (settings.useVirtualPort) {
            std::cout << "Setting up local virtual midi port " << settings.virtualPortName << "\n";
            midiIn.openVirtualPort();
            std::cout << settings.virtualPortName << " port # is " << midiIn.getPort() << '\n';
        }
        else {
            for (std::string portIn : midiIn.getInPortList()) {
                std::cout << "Available InPort: " << portIn << '\n';
                if (portIn.compare(settings.networkPortName) == 0) {
                    midiIn.openPort(settings.networkPortName);
                    if (midiIn.isOpen())
                        std::cout << "Found desired network port and opened it \n";
                }
            }

        }
        midiIn.addListener(this);
    }

    /**
     * ofMidiListener implemented method
     */
    void newMidiMessage(ofxMidiMessage& message){
        unsigned int channel(message.channel - 1);
        if(channel < settings.numChannels){
            channels[channel].processMIDIMessage(message);
        }
    }
    
};

#endif /* MIDIPortNotesState_h */
