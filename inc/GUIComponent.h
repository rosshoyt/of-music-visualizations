#pragma once
#include "ofMain.h"
#include "ofParameter.h"
#include "ofxGui.h"
#include "UID.h"
#include "GUISubComponent.h"
#include "GUIParameterNode.h"

class GUISubComponent;

class GUIComponent : public UID {
public:
	GUIComponent(std::string uid);

	// method which must be implemented by inheriting class.
	// gui parameters should be added to the ofxPanel in this method
	// GUISubComponents should also be added to the ofxPanel in this method
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

	// method which can be implemented to track which GUI Parameters are clicked
	virtual void setupParameterListeners();

	static GUIParameterNode* getLastClickedParameter() {
		return lastClickedParam;
	}

	template<typename T> void addParameterListener(ofParameter<T>& param) {
		eventListeners.push(param.newListener([&](T&) { GUIComponent::clickEvent({ param }); }));
	}

	// TODO fix, only works for ofParameters of the same type
	template<typename T> void addParameterListeners(std::vector<ofParameter<T>>& params) {
		for (const auto& param : params)
			addParameterListener(param);
	}

protected:

	ofxPanel gui;

	std::vector<GUISubComponent*> subComponents;

	const std::string DEFAULT_SETTINGS_FILE_NAME = "settings.xml";

	float menuX = 0, menuY = 0;

	float defaultMenuWidth = 100; // TODO choose better default value
	
	
private:
	ofEventListeners eventListeners;

	bool guiHasBeenSetup = false;

	static GUIParameterNode* lastClickedParam;
	
	void clickEvent(ofAbstractParameter& e) {
		//std::cout << "Click Event for " << e.getName() << '\n';
		lastClickedParam = new GUIParameterNode(e);
	}
};
