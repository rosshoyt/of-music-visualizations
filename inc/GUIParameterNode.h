#pragma once
#include "ofMain.h"

// Class which creates a linked reference to another GUI Parameter, 
// Allows the GUI Parameter to be controlled and scaled, regardless of type.
class GUIParameterNode {
public:

	GUIParameterNode() = default;

	GUIParameterNode(ofAbstractParameter& e);

	void setValue(float percentage);

	std::string getName();
private:
	// possible types of the underlying parameter
	enum ParamType {
		NONE, INT, FLOAT, DOUBLE
	};

	template<typename T> void setValue(float percentage, ofParameter<T>& param);

	// the name of the parameter
	std::string paramName;

	// type of the parameter
	ParamType paramType = NONE;

	ofParameter<int> intParam;
	ofParameter<float> floatParam;
	ofParameter<double> doubleParam;
};