#include "GUIParameterNode.h"

GUIParameterNode::GUIParameterNode(ofAbstractParameter& e) {
	paramName = e.getName();

	// TODO refactor if possible / redesign - could probably use class template arguments,
	// TODO or extend ofParameter
	if (e.type() == typeid(ofParameter<int>).name()) {
		paramType = INT;
		// cast to ofParameter of this type
		auto parRef = e.cast<int>();
		intParam.makeReferenceTo(parRef);
		intParam.setMin(parRef.getMin());
		intParam.setMax(parRef.getMax());
	}
	else if (e.type() == typeid(ofParameter<float>).name()) {
		paramType = FLOAT;
		// cast to ofParameter of this type
		auto parRef = e.cast<float>();
		floatParam.makeReferenceTo(parRef);
		floatParam.setMin(parRef.getMin());
		floatParam.setMax(parRef.getMax());
	}
	else if (e.type() == typeid(ofParameter<double>).name()) {
		paramType = DOUBLE;
		// cast to ofParameter of this type
		auto parRef = e.cast<double>();
		doubleParam.makeReferenceTo(parRef);
		doubleParam.setMin(parRef.getMin());
		doubleParam.setMax(parRef.getMax());
	}
	else if (e.type() == typeid(ofParameter<ofColor>).name()) {
		paramType = COLOR;
		// cast to ofParameter of this type
		auto parRef = e.cast<ofColor>();
		colorParam.makeReferenceTo(parRef);
		//colorParam.setMin(parRef.getMin());
		//colorParam.setMax(parRef.getMax());
	}

	// debug msg
	//std::cout << "Created GUIParameterNode for " << e.getName() << " of type " << paramType << '\n';
}

void GUIParameterNode::setValue(float percentage) {
	switch (paramType) {
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
	case COLOR:
		setColorValue(percentage, colorParam);
		break;
	}
}

std::string GUIParameterNode::getName() {
	return paramName;
}

template<typename T> void GUIParameterNode::setValue(float percentage, ofParameter<T>& param) {
	T min = param.getMin(), max = param.getMax();
 	T range = max - min;
 	T newValue = min + range * percentage;
	// set the new parameter value
	param.set(min + range * percentage);

	// debug msg
	//std::cout << "Setting value for " << paramName << " to percentage " << percentage << " (current value = " << param << ", updated value = " << newValue <<")." << "The min/max were: " << min << "/" << max << "\n";
 }



