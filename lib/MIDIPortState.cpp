#include "MIDIPortState.h"

MIDIPortState::MIDIPortState() : GUIComponent("MIDI Port Info") {
	setupMIDIPortState();
}

void MIDIPortState::setupGUI() {
	gui.clear();
	gui.setName(getUID());

	gui.add(numChannels.set("# MIDI Channels", numChannels, 1, 16));// .numChannels.set("# MIDI Channels", );
	gui.add(useVirtualPort.set("Use Virtual Port", useVirtualPort));
	gui.add(virtualPortName.set("Virtual Port Name", virtualPortName));
	gui.add(networkPortName.set("Network Port Name", networkPortName));
    gui.add(resetMidiPortButton.setup("Reset MIDI Port"));
	gui.add(midiMessageMonitor);// .set("MIDI Data", midiMessageMonitor));

	for (int i = 0; i < numChannels; i++) {
		auto params = channels[i]->getChannelSettings()->params;
		params.setName("Channel " + std::to_string(i + 1) + " Settings");
		std::cout << "MIDIPortState::setupGUI() added channel params with size " << params.size() << "\n";
		gui.add(params);
	}

    resetMidiPortButton.addListener(this, &MIDIPortState::setupOfxMIDIPort);
   
	gui.minimizeAll();

	//for (int i = 0; i < numChannels; i++) {
	//	//Settings* settings = new Settings(i);
	//	gui.add(settings->params);
	//	//perChannelSettings.push_back(settings);
	//}
}

unsigned int MIDIPortState::getNumChannels() {
	return numChannels;
}

/**
* Gets all the notes currently held down or sustained with pedal on the specified midi channel
* @param channel - MIDI Channel Number (1-16)
* TODO Input validation
*/
std::map<int, int> MIDIPortState::getChannelNotes(unsigned int channel) {
	return channels[channel]->getAllNotes();
}

std::vector<std::map<int, int>> MIDIPortState::getAllChannelNotes() {
	std::vector<std::map<int, int>> channelNotesList;
	for (unsigned int i = 0; i < numChannels; ++i) {
		channelNotesList.push_back(getChannelNotes(i));
	}
	return channelNotesList;
}

const std::map<int, std::pair<int, float>> MIDIPortState::getAllNotesDown() {
	// Get the current active MIDI notes for all channels and
	// store in map<midiPitch, <velocity, adsr value>>
	std::map<int, std::pair<int, float>> allNotesDown;
	int channelNum = 0;
	for (auto channelNotes : getAllChannelNotes()) {
		for (auto note : channelNotes) {
			// TODO don't overwrite values when 2 notes are same between channels
			allNotesDown.insert({ note.first,{ note.second, getADSRValue(channelNum, note.first) } });
		}
		++channelNum;
	}
	return allNotesDown;
}

std::vector<std::map<int, std::pair<int, float>>> MIDIPortState::getAllChannelActiveNoteADSRLevels() {
	std::vector<std::map<int, std::pair<int, float>>> ret;
	for (int i = 0; i < numChannels; ++i) {
		ret.push_back(channels[i]->getAllActiveNoteADSRLevels());
	}
	return ret;
}

/**
* Gets the current value of the specified MIDI CC value based on its channel
* @param channel - MIDI Channel Number (0-15)
* @param ccNumber - MIDI CC Number (0-127)
* TODO Input validation
*/
int MIDIPortState::getMIDICCValue(unsigned int channel, int ccNumber) {
	return channels[channel]->tryGetCCValue(ccNumber);
}

float MIDIPortState::getADSRValue(unsigned int channel, int noteNumber) {
	return channels[channel]->getADSRLevel(noteNumber);
}

MIDIChannelSettings* MIDIPortState::getChannelSettings(unsigned int channel) {
	return channels[channel]->getChannelSettings();
}


void MIDIPortState::setupMIDIPortState() {
	std::cout << "In MIDIPortState::setupMIDIPortState(), numChannels = "<< numChannels << "\n";
	validateSettings();
	EnvelopeSettings settings;
	settings.envelopeType = ADSR;
	settings.envSegmentLengths = { 50, 2000, 400 };
	settings.envSegmentLevels = { 0.f, 1.f, .5f };

	Envelope envelope(settings);
	
	MIDIChannelSettings* channelSettings = new MIDIChannelSettings(envelope);


	// initialize channel state map TODO move to separate function and only re-initialize when needed
	for (int i = 0; i < numChannels; ++i) {
		std::cout << "Creating MIDI Channel #" << i+1 << '\n';
		channels.push_back(new MIDIChannelState(new MIDIChannelSettings(envelope)));
	}
	setupOfxMIDIPort();
}

void MIDIPortState::validateSettings() {
	// ensure numChannels is between 1 and 16 (inclusive)
	assert(numChannels >= 1 && numChannels <= 16);
}

void MIDIPortState::setupOfxMIDIPort() {
	std::cout << "Setting up MIDI Port...";
	if (useVirtualPort) {
		std::cout << "... using local virtual midi port name " << virtualPortName << "\n";
		midiIn.openVirtualPort();
		std::cout << virtualPortName << " port # is " << midiIn.getPort() << '\n';
	}
	else {
		std::cout << "... using network midi port\n";
		for (std::string portIn : midiIn.getInPortList()) {
			std::cout << "Available InPort: " << portIn << '\n';
			if (portIn.compare(networkPortName) == 0) {
				midiIn.openPort(networkPortName);
				if (midiIn.isOpen())
					std::cout << "Found network port " << networkPortName << " and opened it.\n";
			}
		}

	}
	midiIn.addListener(this);
}

/**
* ofMidiListener implemented method.
* Passes the midi message to its corresponding channel.
* Converts Channel Number from 1-16 (inclusive) to 0-15 (inclusive)
*/
void MIDIPortState::newMidiMessage(ofxMidiMessage& message) {
	updateMIDIMessageMonitor(message);
	unsigned int channel(message.channel - 1);
	if (channel < numChannels) {
		channels[channel]->processMIDIMessage(message);
	}
}

void MIDIPortState::updateMIDIMessageMonitor(ofxMidiMessage& message) {
	int descriptionMsgSize = 22;
	std::string text, value;
	text.append("Ch#" + std::to_string(message.channel) + " ");
	// TODO refactor
	switch (message.status) {
	case MIDI_NOTE_ON:
		
		text.append("Note On");
		value = std::to_string(message.pitch);
		break;
	case MIDI_NOTE_OFF:
		text.append("Note Off");
		value = std::to_string(message.pitch);
		break;
	case MIDI_CONTROL_CHANGE:
		text.append("CC#" + std::to_string(message.control));
		value = std::to_string(message.value);
	default:
		;
	}
	text.resize(descriptionMsgSize - value.size(), ' ');
	text.append(value);
	
	midiMessageMonitor = text;
}
