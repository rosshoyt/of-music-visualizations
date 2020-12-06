#pragma once
#include "ofMain.h"
#include <vector>
#include "spline.h"
#include "Utils.h"

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

	// TODO combine with getLevel method? could use boolean flag
	float getLevelForRelativeTime(double timeSinceStartOfThisEnvelopeSegment); 

	double getLength();

	float getStartingLevel();

	bool containsTime(double elapsedTimeMS);

	// Public GUI Sliders
	ofParameter<float> splineIntensitySlider;
	ofParameter<double> lengthSlider;

private:

	void init();

	std::vector<double> getSplineYControlsWithIntensity();

	// PRIVATE Fields
	EnvelopeSegmentSettings settings;

	// Derived values
	double totalLength;

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
	std::vector<double> envSegmentLevels  = { 0.f, 1.f, .5f };
	EnvelopeType envelopeType = EnvelopeType::ADSR;
};

class Envelope {
public:
	Envelope(EnvelopeSettings envelopeSettings);

	// no-args constructor which sets default envelopeADR settings
	Envelope();

	EnvelopeType getEnvelopeType();

	// method which gets the value of the envelope based on the time since start.
	// returns -1 if the time is not part of the envelope
	float getLevel(double timeSinceNoteStart, double timeSinceNoteOff);

	float getLevel(long timeSinceNoteStart, long timeSinceNoteOff);

	double getLength();

	double getAttackLength();

	double getDecayLength();

	double getReleaseLength();

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
	EnvelopeNode(Envelope* envelope);

	float getLevel(long currentTimeMS);

	float getLevel();

	void start(int velocity);

	// function called when a note is pressed down (via Note On)
	void start();
	
	// function called when a note is released (via Note Off or Sustain Pedal Off)
	void stop();

	long getLastStartTimeMS();

	long getLastStopTimeMS();

	int getLastNoteOnVelocity();

private:
	Envelope* envelope;

	long lastStart = 0, lastStop = 0;
	
	int lastNoteOnVelocity = 0;
};