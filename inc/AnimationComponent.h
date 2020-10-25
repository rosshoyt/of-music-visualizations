#pragma once
#include <string>
#include "MIDIPortState.h"
#include "ofMain.h"

class AnimationComponent {
public:
	// TODO could move 'MIDI' code to an abstract class
	AnimationComponent(MIDIPortState* midiPortState, std::string uid) : midiPortState(midiPortState), uid(uid) {

	}

	static void setMenuXY(float x, float y) {

	}

	// method which should be called to initialize the AnimationComponent and prepare it to display at some point in the future.
	// memory allocations, expensive 1-time  initializations and file operations are examples that should be implemented in setup()
	virtual void setup() = 0;

	// method which should be called first when the AnimationComponent is changed-to. 
	// for example, when it is selected to be displayed from the dropdown menu
	// sets up any destructive rendering settings which may affect other animations
	virtual void prepare() {}

	// method which should be called first when an animation is being changed-from
	// for exmaple, when it is the current animation and another is selected from the dropdown menu
	virtual void takedown() {}

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
	float menuX, menuY;

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