#include "MIDIChannelState.h"

MidiCCNode::MidiCCNode() : value(), mtx() {}

int MidiCCNode::getValue() {
	Lock lck(mtx, std::defer_lock);
	lck.lock();
	int cpy = value;
	lck.unlock();
	return cpy;
}

void MidiCCNode::setValue(int value) {
	Lock lck(mtx, std::defer_lock);
	lck.lock();
	this->value = value;
	lck.unlock();
}


MIDIChannelState::MIDIChannelState() : notesHeldDown(), notesSustained(), midiCCState(128), adsrStates(), sustained(false), mtx(), mtxSusNotes() {
	settings = new Settings();
	
	// TODO move to init() function
	//ADSR pianoADSR;
	for (int i = 0; i < 128; i++) {
		//NoteADSRState* adsrNode = new NoteADSRState(pianoADSR);
		EnvelopeNode* envelopeNode = new EnvelopeNode(&settings->volumeEnvelope);
		adsrStates.push_back(envelopeNode);
	}
}

void MIDIChannelState::processMIDIMessage(ofxMidiMessage& message) {
	switch (message.status) {
	case MIDI_NOTE_ON:
		tryAddNoteOn(message.pitch, message.velocity);
		// TODO refactor channel to process each midi message independantly
		break;

	case MIDI_NOTE_OFF:
		tryAddNoteOff(message.pitch);
		break;

	case MIDI_CONTROL_CHANGE:
		processMIDICC(message);

		break;
	}
}

void MIDIChannelState::processMIDICC(ofxMidiMessage& message) {
	switch (message.control) {
	case 1:
		tryAddMIDICC(message.control, message.value);
		break;
	case 64:
		switch (message.value) {
		case 0:
			//std::cout<< "MIDI Control Change # " << message.control << " value = " << message.value << '\n';
			setSustainPedalOff();
			break;

		case 127:

			setSustainPedalOn();
			break;
		}
	}
}

void MIDIChannelState::tryAddNoteOn(int note, int velocity) {
	Lock lck(mtx, std::defer_lock);
	lck.lock();
	// insert or update new note/velocity pair
	notesHeldDown[note] = velocity;
	lck.unlock();

	adsrStates[note]->start();
}

void MIDIChannelState::tryAddNoteOff(int note) {
	// We may want to continue to hear the note if the sustain pedal is down, so track it
	if (sustained) {
		// Move note to sustainedNotes
		// first get the note's earliest occuring NoteOn Velocity
		// TODO could cause exception if key not exists in the map (cased by corrupted MIDI data)
		int noteOnVel = notesHeldDown.at(note); // 'concurrent access is safe' @s`ee http://www.cplusplus.com/reference/map/map/at/
												// add to sustainedNotes
		Lock lck2(mtxSusNotes, std::defer_lock);
		lck2.lock();
		// insert or update note value
		notesSustained[note] = noteOnVel;
		lck2.unlock();

	}
	else {
		// If sustain pedal wasn't down, clear the ADSR curve because we will be removing it
		// TODO can add sus pedal tracking to the ADSR class
		adsrStates[note]->stop();
	}
	// Remove note from list of held-down notes
	Lock lck(mtx, std::defer_lock);
	lck.lock();
	notesHeldDown.erase(note);
	lck.unlock();

}

void MIDIChannelState::setSustainPedalOn() {
	sustained.store(true);
}

void MIDIChannelState::setSustainPedalOff() {
	if (sustained) {
		// clear ADSR curves of all the notes that were
		// only being sustained
		for (const auto& nnvp : notesSustained)
			adsrStates[nnvp.first]->stop();


		// clear all notes from sustainedNotes
		Lock lck(mtxSusNotes, std::defer_lock);
		lck.lock();
		notesSustained.clear();
		lck.unlock();

		sustained.store(false);

	}
}

// TODO input validation

float MIDIChannelState::getADSRLevel(int note) {
	return adsrStates[note]->getLevel();
}

std::map<int, int> MIDIChannelState::getNotes() {
	Lock lck(mtx, std::defer_lock);
	lck.lock();
	// copy note map
	auto ret = notesHeldDown;
	lck.unlock();
	return ret;
}

std::map<int, int> MIDIChannelState::getSustainedNotes() {
	Lock lck(mtxSusNotes, std::defer_lock);
	lck.lock();
	// copy sus notes map
	auto ret = notesSustained;
	lck.unlock();
	return ret;
}

std::map<int, int> MIDIChannelState::getAllNotes() {
	auto notes = getNotes();
	auto susNotes = getSustainedNotes();
	notes.insert(susNotes.begin(), susNotes.end());
	return notes;
}

int MIDIChannelState::getNumNotes() {
	Lock lck(mtx, std::defer_lock);
	lck.lock();
	int n = notesHeldDown.size();
	lck.unlock();
	return n;
}

void MIDIChannelState::tryAddMIDICC(int midiCC, int value) {
	//std::cout << "Recieved midi CC #1 val = " << value << '\n';
	Lock lck(mtx, std::defer_lock);
	lck.lock();
	midiCCState[midiCC].setValue(value);
	lck.unlock();
}

int MIDIChannelState::tryGetCCValue(int ccNo) {
	Lock lck(mtx, std::defer_lock);
	lck.lock();
	int cpy = midiCCState[ccNo].getValue();
	lck.unlock();
	//std::cout << "returning cc#" << ccNo << " value = " << cpy << '\n';
	return cpy;
}

bool MIDIChannelState::sustainPedalIsDown() {
	return sustained.load();
}


