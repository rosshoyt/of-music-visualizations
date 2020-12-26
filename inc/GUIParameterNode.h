#pragma once
#include "ofMain.h"

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
	
	vector<ofColor> defaultCols = { ofColor::red, ofColor::blue, ofColor::green };
	
	inline void setColorValue(float percentage, ofParameter<ofColor>& colorParam_) {

		

		ofColor c1, c2;
		float curr = 0, divisor = 3; // 3 - way color interpolation
		float increment = 1.f / divisor;
		
		
		

		int colorIndex = -1;
		do{
			++colorIndex;
			curr += increment;
		} while (curr < percentage);
		c1 = defaultCols[colorIndex];

		float amountOfLerp = percentage - curr;

		colorParam_.set(c1.getLerped(c2, amountOfLerp));
	}

	// the name of the parameter
	std::string paramName;

	
	// possible types of the underlying parameter
	enum ParamType {
		NONE, INT, FLOAT, DOUBLE, COLOR
	};
	ParamType paramType = NONE; // default type when no parameter has been set
	

	ofParameter<int> intParam;
	ofParameter<float> floatParam;
	ofParameter<double> doubleParam;
	ofParameter<ofColor> colorParam;


	

};