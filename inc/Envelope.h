#pragma once
#include "ofMain.h"
#include <vector>
#include "spline.h"

//enum EnvelopeSegmentType { ATTACK, DECAY, SUSTAIN, RELEASE };


// TODO - template parameterize all classes
//template <class T>
//class EnvelopeSegmentSettings {
//	T start, startLevel;
//public:
//	EnvelopeSegmentSettings(T first, T second)
//	{
//		a = first; b = second;
//	}
//	T getmax();
//};
//
//template <class T>
//T mypair<T>::getmax()
//{
//	T retval;
//	retval = a > b ? a : b;
//	return retval;
//}

// TODO implement in MIDIChannelState
//class EnvelopeNode {
//	std::shared_ptr<Envelope> envelope;
//};


class EnvelopeSegmentSettings {
public:
	double start = 0, startLevel = 0, end = 500, endLevel = 1;
};

class EnvelopeSegment {
public:
	EnvelopeSegment(EnvelopeSegmentSettings settings);

	// TODO could refactor - store calculated value to avoid calculating redundantly?
	// Must call init() before
	float getLevel(double xVal);

	double getLength();

	// TODO (only if implementation completed using real-time values for Segment time values)
	bool containsTime(double elapsedTimeMS);

	/*void setStart(double start) { settings.start = start; }

	void setEnd(double end) { end = end; }

	void setStartLevel(double startLevel) { settings.startLevel = startLevel; }

	void setEndLevel(double endLevel) { endLevel = endLevel; }*/

	void init();

	
	//bool containsTime(double elapsedTimeMS) {}

	// Public GUI Slider
	ofParameter<float> splineIntensitySlider;

private:

	// PRIVATE Fields
	EnvelopeSegmentSettings settings;

	// Derived values
	double totalLength;

	// enum type of envelope segment
	//EnvelopeSegmentType type;
	// spline control values
	std::vector<double> splineControlX, splineControlY;

	// constants
	const int NUM_INTERNAL_CONTROL_SEGMENTS = 3;
	const float MAX_SPLINE_CONTROL_PERC = 0.88888888; // so that control points don't put spline above/below target val

};

enum EnvelopeType { ADR, ADSR };


class EnvelopeSettings {
public:
	std::vector<double> envSegmentLengths = { 30, 1000, 500 };
	std::vector<double> envSegmentLevels  = { 0.f, 1.f, .25 };
	EnvelopeType envelopeType = EnvelopeType::ADR;
};

class Envelope {
public:
	
	Envelope(EnvelopeSettings envelopeSettings);

	// no-args constructor sets default envelope settings
	Envelope();

	EnvelopeType getEnvelopeType();

	float getLevel(double time);

	float getLevel(long time);

	double getLength();

	ofParameterGroup guiParams;


private:

	void init();
	std::vector<EnvelopeSegment*> envelopeSegments;
	EnvelopeSettings envelopeSettings;

	double totalLength;

	







};