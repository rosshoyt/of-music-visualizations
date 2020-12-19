//
//  MIDIPortNotesState.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 8/9/20.
//

#ifndef MIDIPortNotesState_h
#define MIDIPortNotesState_h
#include "ofxMidi.h"
#include "ofxGui.h"
#include "MIDIChannelState.h"
#include "GUIComponent.h"
#include <assert.h>


class MIDIPortState : public ofxMidiListener, public GUIComponent {
public:
    MIDIPortState();

    void setupGUI() override;
   
    unsigned int getNumChannels();

    /**
     * Gets all the notes currently held down or sustained with pedal on the specified midi channel
     * @param channel - MIDI Channel Number (0-15)
     * TODO Input validation
     */
    std::map<int, int> getChannelNotes(unsigned int channel);

    std::vector<std::map<int, int>> getAllChannelNotes();
    
    const std::map<int, std::pair<int, float>> getAllNotesDown();

    std::vector<std::map<int, std::pair<int, float>>> getAllChannelActiveNoteADSRLevels();

    /**
     * Gets the current value of the specified MIDI CC value based on its channel
     * @param channel - MIDI Channel Number (0-15)
     * @param ccNumber - MIDI CC Number (0-127)
     * TODO Input validation
     */
    int getMIDICCValue(unsigned int channel, int ccNumber);
    
    float getADSRValue(unsigned int channel, int noteNumber);


    MIDIChannelSettings* getChannelSettings(unsigned int channel);

    void resetNotes();

private:
    // Midi Input port
    ofxMidiIn midiIn;
    // GUI parameters
    // flag set to true if using local MIDI port, false if using network MIDI port
    ofParameter<bool> useVirtualPort = false;
    // names of local and network MIDI ports
    ofParameter<std::string> networkPortName = std::string("Network Session 1"), virtualPortName = std::string("ofxMidiIn Input");
    // How many midi channels to listen to on the port (1 - 16)
    ofParameter<unsigned int> numChannels = 10;
    
    ofxButton resetMidiPortButton, resetNotesButton;
    
    // Display field for the most recent midi message
    ofParameter<std::string> midiMessageMonitor = std::string("No MIDI Messages Detected");
    
    // Array of channel note states (TODO refactor to std::vector)
    std::vector<MIDIChannelState*> channels;
    
    // Private methods
    void setupMIDIPortState();

    void validateSettings();

    void setupOfxMIDIPort();

    /**
     * ofxMidiListener implemented method
     */
    void newMidiMessage(ofxMidiMessage& message);

    void updateMIDIMessageMonitor(ofxMidiMessage& message);
    
    
};
#endif /* MIDIPortNotesState_h */
