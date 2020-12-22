#pragma once
#include "ofMain.h"
#include "ofParameter.h"
#include "ofxGui.h"
#include "UID.h"

class GUIParameterNode {
public:
	enum ParamType {
		NONE, INT, FLOAT, DOUBLE
	};

	GUIParameterNode() = default;

	GUIParameterNode(ofAbstractParameter& e) { 
		if (e.type() == typeid(ofParameter<int>).name()) {
			intParam = e.cast<int>();
			paramType = INT;
		}
		else if (e.type() == typeid(ofParameter<float>).name()) {
			floatParam = e.cast<float>();
			paramType = FLOAT;
		}
		else if (e.type() == typeid(ofParameter<double>).name()) {
			doubleParam = e.cast<double>();
			paramType = DOUBLE;
		}
		
		std::cout << "Created GUIParameterNode for " << e.getName() << " of type " << paramType << '\n';
	}

	void setValue(float percentage) {
		switch (paramType){
		case NONE:
			return;
		case INT:
			setValue<int>(percentage, intParam);
			break;
		case FLOAT:
			setValue<float>(percentage, floatParam);
			break;
		case DOUBLE:
			setValue<double>(percentage, doubleParam);
			break;
		}
	}

	std::string getName(){
		std::string ret;
		switch (paramType) {
		case INT:
			ret = intParam.getName();
		case FLOAT:
			ret = floatParam.getName();
		case DOUBLE:
			ret = doubleParam.getName();
		}
		return ret;
	}
private:
	template<typename T> void setValue(float percentage, ofParameter<T>& param) {
		
		T min = intParam.getMin(), max = intParam.getMax();
		T range = max - min;
		T newValue = min + range * percentage;
		std::cout << "Setting value for " << param.getName() << " to percentage " << percentage << " (current value = " << param << ", updated value = " << newValue <<")." << "The min/max were: " << min << "/" << max << "\n";
		//param.set(min + range * percentage);
	}

	ParamType paramType = NONE;

	ofParameter<int> intParam = 0;
	ofParameter<double> doubleParam = 0;
	ofParameter<float> floatParam = 0;
};

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

	// method which can be implemented to track which GUI Parameters are clicked
	virtual void setupParameterListeners();

	static GUIParameterNode* getLastClickedParameter() {
		return lastClickedParam;
	}
	
protected:

	ofxPanel gui;

	const std::string DEFAULT_SETTINGS_FILE_NAME = "settings.xml";

	float menuX = 0, menuY = 0;

	float defaultMenuWidth = 100; // TODO choose better default value

	template<typename T> void addParameterListener(ofParameter<T>& param) {
		eventListeners.push(param.newListener([&](T&) { GUIComponent::clickEvent({ param }); }));
	}
	
private:
	ofEventListeners eventListeners;

	static GUIParameterNode* lastClickedParam;
	
	void clickEvent(ofAbstractParameter& e) {
		std::cout << "Click Event for " << e.getName() << '\n';
		lastClickedParam = new GUIParameterNode(e);
	}
};

