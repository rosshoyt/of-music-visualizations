#pragma once
#include "GUIComponent.h"
#include "UID.h"
#include "MIDIPortState.h"
#include <string>

// TODO Change to MIDIPortStateController
class MIDIPortStateGUI : public GUIComponent {
public:
	MIDIPortStateGUI() : GUIComponent("MIDI Port Info") {
		// setup the MIDI port
		midiPortSettings.numChannels = 12;
		midiPortState.setupMIDIPortState(midiPortSettings);
	}

	
	void setupGUI() override {
		gui.setName(getUID());
		
		gui.add(midiPortSettings.numChannels.set("# MIDI Channels", midiPortSettings.numChannels, 1, 16));// .numChannels.set("# MIDI Channels", );
		gui.add(midiPortSettings.useVirtualPort.set("Use Virtual Port", midiPortSettings.useVirtualPort));
		gui.add(midiPortSettings.virtualPortName.set("Virtual Port Name", midiPortSettings.virtualPortName));
		gui.add(midiPortSettings.networkPortName.set("Network Port Name", midiPortSettings.networkPortName));
		std::cout << "Setting up MIDIPortStateGUI - num channels = " << midiPortSettings.numChannels << "\n";
	}

	MIDIPortState& getMIDIPortState() {
		return midiPortState;
	}

private:
	//std::string _uid = "MIDIPortStateGUI"; // TODO fix and use this (member initializers)
	MIDIPortStateSettings midiPortSettings;
	MIDIPortState midiPortState;
	
};

