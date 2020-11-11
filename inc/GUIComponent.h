#pragma once
#include "ofMain.h"
#include "ofxGui.h"

class GUIComponent {
public:
	GUIComponent();

	// method which must be implemented by inheriting class.
	// gui parameters should be added to the ofxPanel in this method
	virtual void setupGUI() = 0;
	
	void drawGUI();

	void setMenuXY(float x, float y);

	float getMenuHeight();

	float getMenuWidth();
protected:
	ofxPanel gui;

	// TODO refactor, move GUI code to separate class
	float menuX, menuY;

};

