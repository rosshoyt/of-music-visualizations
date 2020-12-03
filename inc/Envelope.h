#pragma once
#include "ofMain.h"
#include <vector>
#include "spline.h"
#include "Utils.h"

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




class EnvelopeSegmentSettings {
public:
	double start = 0, startLevel = 0, end = 500, endLevel = 1;
};

class EnvelopeSegment {
public:
	EnvelopeSegment(EnvelopeSegmentSettings settings);

	// TODO could refactor - store calculated value to avoid calculating redundantly?
	// Must call containsTime() to ensure segment contains valid value for time value
	float getLevel(double timeSinceNoteStart);

	

	double getLength();

	float getStartingLevel() {
		return settings.startLevel;
	}

	// TODO combine with getLevel method? could use boolean flag
	float getLevelForRelativeTime(double timeSinceStartOfThisEnvelopeSegment) {
		if (timeSinceStartOfThisEnvelopeSegment >= 0 && timeSinceStartOfThisEnvelopeSegment < totalLength) {
			std::vector<double> xTemps(splineControlXRelative), yTemps = getSplineYControlsWithIntensity();
			tk::spline spline;
			spline.set_points(xTemps, yTemps);
			return spline(timeSinceStartOfThisEnvelopeSegment);
		}
		
		return 0;
	}


	// TODO (only if implementation completed using real-timeSinceNoteStart values for Segment timeSinceNoteStart values)
	bool containsTime(double elapsedTimeMS);

	/*void setStart(double start) { settings.start = start; }

	void setEnd(double end) { end = end; }

	void setStartLevel(double startLevel) { settings.startLevel = startLevel; }

	void setEndLevel(double endLevel) { endLevel = endLevel; }*/

	
	
	//bool containsTime(double elapsedTimeMS) {}

	// Public GUI Slider
	ofParameter<float> splineIntensitySlider;

private:

	void init();

	std::vector<double> getSplineYControlsWithIntensity() {
		std::vector<double> yTemps(splineControlY);
		yTemps[0] -= splineIntensitySlider;
		yTemps[2] += splineIntensitySlider;
		yTemps[3] += splineIntensitySlider;
		yTemps[5] -= splineIntensitySlider;
		return yTemps;
	}

	// PRIVATE Fields
	EnvelopeSegmentSettings settings;

	// Derived values
	double totalLength;

	// enum type of envelopeADR segment
	//EnvelopeSegmentType type;
	// spline control values
	std::vector<double> splineControlX, splineControlY;

	// Relative control values that has envelope start time as 0
	std::vector<double> splineControlXRelative;

	// constants
	const int NUM_INTERNAL_CONTROL_SEGMENTS = 3;
	const float MAX_SPLINE_CONTROL_PERC = 0.88888888; // so that control points don't put spline above/below target val

};

enum EnvelopeType { ADR, ADSR };


class EnvelopeSettings {
public:
	std::vector<double> envSegmentLengths = { 30, 2000, 1000 };
	std::vector<double> envSegmentLevels  = { 0.f, 1.f, .25 };
	EnvelopeType envelopeType = EnvelopeType::ADSR;
};

class Envelope {
public:
	
	Envelope(EnvelopeSettings envelopeSettings);

	// no-args constructor sets default envelopeADR settings
	Envelope();

	EnvelopeType getEnvelopeType();

	// method which gets the value of the envelope based on the time since start.
	// returns -1 if the time is not part of the envelope
	float getLevel(double timeSinceNoteStart, double timeSinceNoteOff);

	float getLevel(long timeSinceNoteStart, long timeSinceNoteOff);

	double getLength();

	double getAttackLength() {
		return envelopeSegments[0]->getLength();
	}

	double getDecayLength() {
		return envelopeSegments[1]->getLength();
	}

	double getReleaseLength() {
		return envelopeSegments.back()->getLength();
	}

	ofParameterGroup guiParams;


private:
	void init();


	std::vector<EnvelopeSegment*> envelopeSegments;
	EnvelopeSettings envelopeSettings;

	double totalLength;
};


// Class which manages state information for an object using an envelope
class EnvelopeNode {
public:
	EnvelopeNode(Envelope* envelope) : envelope(envelope) {

	}

	float getLevel(long currentTimeMS) {
		/*if (lastStart > lastStop) {
			return 0;
		}*/
		
		return envelope->getLevel(currentTimeMS - lastStart, currentTimeMS - lastStop);
	}

	float getLevel() {
		return getLevel(ofGetSystemTimeMillis());
	}


	// function called when a note is pressed down (via Note On)
	void start() {
		// TODO
		// deal with what happens when start() called before stop() cancels previous note
		// stop();
		lastStart = ofGetSystemTimeMillis();
		std:cout << "Starting Node at " << lastStart << ", time since last note released = " << lastStart - lastStop <<"\n";
	}
	
	// function called when a note is released (via Note Off or Sustain Pedal Off)
	void stop() {
		lastStop = ofGetSystemTimeMillis();
		std:cout << "Stopping Node at " << lastStop << ", length held = " << lastStop - lastStart << "\n";

	}

	long getLastStartTimeMS() {
		return lastStart;
	}

	long getLastStopTimeMS() {
		return lastStop;
	}
private:
	Envelope* envelope;

	long lastStart = 0, lastStop = 0;
	
};