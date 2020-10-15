#pragma once
#include <string>
#include "MIDIPortState.h"

class AnimationComponent {
public:
	AnimationComponent(MIDIPortState* midiPortState, std::string uid) : midiPortState(midiPortState), uid(uid) {

	}

	virtual void setup() = 0;

	virtual void update() = 0;

	virtual void draw() = 0;

	virtual void drawGUI() = 0;

	void drawBackground() {}

	const std::string& getUID() {
		return uid;
	}
	void setMenuXY(int x, int y) {
		menuX = x, menuY = y;
	}
	void setBackgroundColor(ofColor color) {
		backgroundColor = color;
	}

	ofColor getBackgroundColor() {
		return backgroundColor;
	}
private:
	std::string uid;

protected:
	// TODO 
	// int displayWidth, displayHeight // to track the sub-window space that the animation component displays on

	MIDIPortState* midiPortState;

	// TODO refactor, move menu code to the ofApp and create framework for sub-apps to communicate/update GUI
	int menuX, menuY;

	ofColor backgroundColor;

	const std::map<int, std::pair<int, float>> getAllNotesDown() {
		// Get the current active MIDI notes for all channels and
		// store in map<midiPitch, <velocity, adsr value>>
		std::map<int, std::pair<int, float>> allNotesDown;
		int channelNum = 0;
		for (auto channelNotes : midiPortState->getAllChannelNotes()) {
			for (auto note : channelNotes) {
				// TODO don't overwrite values when 2 notes are same between channels
				allNotesDown.insert({ note.first, { note.second, midiPortState->getADSRValue(channelNum, note.first) } });
			}
			++channelNum;
		}
		return allNotesDown;
	}
	


};