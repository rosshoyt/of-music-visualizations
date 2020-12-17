#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "UID.h"

class GUIComponent : public UID {
public:
	GUIComponent(std::string uid);

	// method which must be implemented by inheriting class.
	// gui parameters should be added to the ofxPanel in this method
	virtual void setupGUI() = 0;
	
	void setGUIName(std::string name);

	std::string getGUIName();

	virtual void drawGUI();
	
	// TODO allow user to specify a different settings save file name
	void loadSavedSettings(); 

	virtual void setMenuXY(float x, float y);

	virtual float getMenuHeight();

	float getMenuWidth();

	void setDefaultMenuWidth(float width);
protected:
	ofxPanel gui;

	const std::string DEFAULT_SETTINGS_FILE_NAME = "settings.xml";

	float menuX = 0, menuY = 0;

	float defaultMenuWidth = 100; // TODO choose better default value
};

