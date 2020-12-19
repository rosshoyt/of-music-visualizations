#pragma once
#include "ofMain.h"
#include <vector>
#include "spline.h"
#include "Utils.h"

class Envelope {
public:
	// no-args constructor which sets default envelopeADR settings
	Envelope();

	float getLevel(long timeSinceStart, long timeSinceEnd, float lastNoteLevel = 0);

	double getLength();

	double getAttackLength();

	double getDecayLength();

	double getReleaseLength();

	// public GUI subgroup
	ofParameterGroup guiParams;

private:
	// GUI params
	ofParameter<double> lengthA = 250, lengthD = 1500, lengthR = 250, levelA = 1, levelD = .5, splineA = 0, splineD = 0, splineR = 0;
	ofParameter<bool> sustain = false;
	// gui sub-sub groups;
	ofParameterGroup attackParams, decayParams, releaseParams;
	
    const double MIN_SEG_LENGTH = 0, MAX_SEG_LENGTH = 20000;
    
    const float MAX_SPLINE_CONTROL_PERC = 0.88888888; // so that control points don't put spline above/below target val
    
    
	float getLevelFromSegment(double segTimeElapsed, double lengthSeg, double levelSegStart, double levelSegEnd, double levelSpline);

	void init();

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

	void reset();

private:
	Envelope* envelope;

	long lastStart = 0, lastStop = 0;
	
	int lastNoteOnVelocity = 0;

	float lastLevel = 0;
};
