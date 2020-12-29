#pragma once
#include "ofMain.h"
#include "Utils.h"
#include <cmath> // std::fmod

// Class which creates a linked reference to another GUI Parameter, 
// Allows the GUI Parameter to be controlled and scaled, regardless of type.
class GUIParameterNode {
public:
	GUIParameterNode() = default;

	GUIParameterNode(ofAbstractParameter& e);

	void setValue(float percentage);

	// void resetValueToDefault(); // TODO Implement tracking of original value

	std::string getName();
private:
	template<typename T> void setValue(float percentage, ofParameter<T>& param);

	// TODO optimize/refactor
	inline void setColorValue(float percentage, ofParameter<ofColor>& colorParam_);

	// the name of the parameter
	std::string paramName;

	// possible types of the underlying parameter
	enum ParamType {
		NONE, INT, FLOAT, DOUBLE, COLOR
	};

	ParamType paramType = NONE; // default type of NONE when no parameter has been set
	
	// the underlying parameters (only one is used per object)
	ofParameter<int> intParam;
	ofParameter<float> floatParam;
	ofParameter<double> doubleParam;
	ofParameter<ofColor> colorParam;
};