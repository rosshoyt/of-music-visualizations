#pragma once
#include <string>
#include "ofMain.h"
#include "ofxGui.h"


class AnimationComponent {
public:
	// TODO could move 'MIDI' code to an abstract class
	AnimationComponent(std::string uid) : uid(uid) {

	}
	
	void setAnimationDimensions(float width, float height) {
		animationWidth = width, animationHeight = height;
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

	virtual void resized(int w, int h) {}

	const std::string& getUID() {
		return uid;
	}

	void setMenuXY(float x, float y) {
		menuX = x, menuY = y;
	}
	void setMenuXY(int x, int y) {
		menuX = x, menuY = y;
	}

	/*void setBackgroundColor(ofColor color) {
		backgroundColor = color;
	}

	ofColor getBackgroundColor() {
		return backgroundColor;
	}*/


protected:

	float animationWidth, animationHeight; // to track the sub-window space that the animation component displays on

	// TODO refactor, move GUI code to separate class
	float menuX, menuY;
	//ofxPanel gui;

	//ofColor backgroundColor;
	
private:
	std::string uid;
};