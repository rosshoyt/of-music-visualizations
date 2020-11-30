#pragma once
#include "Utils.h"
#include "MIDIAnimationComponent.h"
#include "spline.h"
#include "ofMain.h"
#include "ofUtils.h"
#include <map>
#include <algorithm>
#include "ofxGui.h"

enum EnvelopeSegmentType {
	ATTACK, DECAY, SUSTAIN, RELEASE
};

class EnvelopeSegment {
public:
	EnvelopeSegment(EnvelopeSegmentType type, double start, double startLevel, double end, double endLevel) : type(type), start(start), startLevel(startLevel), end(end), endLevel(endLevel) {
		init();
	}

	// Public GUI Sliders
	ofParameter<float> splineIntensitySlider;

	// TODO refactor - store calculated value to avoid calculating redundantly?
	float getLevel(double xVal) {
		tk::spline spline;
		std::vector<double> xTemps(splineControlX), yTemps(splineControlY);

		yTemps[0] -= splineIntensitySlider;
		yTemps[2] += splineIntensitySlider;
		yTemps[3] += splineIntensitySlider;
 		yTemps[5] -= splineIntensitySlider;

		spline.set_points(xTemps, yTemps);

		return spline(xVal);
	}

	double getTotalLength() {
		return totalLength;
	}

	// TODO (only if implementation completed using real-time values for Segment time values)
	//bool containsTime(long elapsedTimeMS) {}
	//bool containsTime(double elapsedTimeMS) {}
private:

	void init() {
		// set derived fields
		totalLength = end - start;

		// setup 6-point control spline in default state (linear)
		double xStepSize = totalLength / NUM_INTERNAL_CONTROL_SEGMENTS;
		double yStepSize = (endLevel - startLevel) / NUM_INTERNAL_CONTROL_SEGMENTS;
		double yVal = startLevel - yStepSize;
		double xVal = start - xStepSize;
		for (int i = 0; i < 6; ++i, xVal += xStepSize, yVal += yStepSize) {
			splineControlY.push_back(yVal);
			splineControlX.push_back(xVal);
			//std::cout << "Xval = "<< xVal << " Yval = " << yVal << '\n';
		}
		// setup intensity slider
		splineIntensitySlider.set("Spline Intensity", 0, -yStepSize * MAX_SPLINE_CONTROL_PERC, yStepSize * MAX_SPLINE_CONTROL_PERC);
	}

	// PRIVATE Fields
	
	// Segment values
	double start = 0;
	double end = 1;
	double startLevel = 0;
	double endLevel = 1;

	// Derived values
	double totalLength;

	// enum type of envelope segment
	EnvelopeSegmentType type;
	// spline control values
	std::vector<double> splineControlX, splineControlY;

	// constants
	const int NUM_INTERNAL_CONTROL_SEGMENTS = 3;
	const float MAX_SPLINE_CONTROL_PERC = 0.88888888; // so that control points don't put spline above/below target val

};

// TODO
//class ADSRCurve {
//	
//	EnvelopeSegment attack, sustain, decay, release;
//};
//enum ADSRCurveType {
//ADR, ADSR
//};

class ADSRVisualizer : public MIDIAnimationComponent {
public:
	ADSRVisualizer(std::string uid = "ADSR Visualizer");

	void setup();

	void setupGUI() override; 

	void prepare();

	void takedown();

	void update();	
	
	void draw();

private:
	EnvelopeSegment attackSegment;

	// display params
	uint64_t colorTimeMS = 100; // in ms
	uint64_t lastColorChange = 0;

	int numPoints = 128;

	std::vector<ofColor> colors;
	ofColor currentColor;

	ofParameter<double> circleSize;
	ofParameter<int> changeSpeed;
	//int minYToChange = 1, maxYToChange = scYDefaults.size() - 2, currentYToChange = minYToChange;
	
	// private methods
	bool shouldChange();

};