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
	}

	// debug
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

	// debug
	//std::cout << "Setting value for " << paramName << " to percentage " << percentage << " (current value = " << param << ", updated value = " << newValue <<")." << "The min/max were: " << min << "/" << max << "\n";
}

inline void GUIParameterNode::setColorValue(float percentage, ofParameter<ofColor>& colorParam_) {

	int r, g, b, a;

	// set the alpha value to its current value
	a = colorParam_.get().a;

	float sectionSize = 1.f / 6.f;

	int section = percentage / sectionSize;

	int interpolateStart, interpolateEnd;

	// find if we are interpolating up or down based on if we're in an even or odd multiple of 1 / 6
	switch (section % 2)
	{
	case 0:
		interpolateStart = 0;
		interpolateEnd = 255;
		break;
	case 1:
		interpolateStart = 255;
		interpolateEnd = 0;
		break;
	}

	float interpolatedValue = utils::math::lerp(interpolateStart, interpolateEnd, std::fmod(percentage, sectionSize) / sectionSize);

	switch (section)
	{
	case 0:
		r = 255;
		g = 0;
		b = interpolatedValue;
		break;
	case 1:
		r = interpolatedValue;
		g = 0;
		b = 255;
		break;
	case 2:
		r = 0;
		g = interpolatedValue;
		b = 255;
		break;
	case 3:
		r = 0;
		g = 255;
		b = interpolatedValue;
		break;
	case 4:
		r = interpolatedValue;
		g = 255;
		b = 0;
		break;
	case 5:
		r = 255;
		g = interpolatedValue;
		b = 0;
		break;
	case 6: // only happens when MIDI CC is at 100%
		r = 255;
		g = 0;
		b = 0;
		break;
	default:
		r = 255;
		g = 255;
		b = 255;
		break;
	}
	colorParam_.set(ofColor(r, g, b, a));
}





