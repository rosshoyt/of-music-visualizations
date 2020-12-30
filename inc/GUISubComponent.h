#pragma once
#include "GUIComponent.h"
#include "ofMain.h"

class GUIComponent;

class GUISubComponent {

public:
	GUISubComponent(GUIComponent* parent);

	virtual void setupParamGroup();
	
	virtual void setupParameterListeners();
	
	ofParameterGroup& getParamGroup();
protected:
	ofParameterGroup paramGroup;

	GUIComponent* parent;
};