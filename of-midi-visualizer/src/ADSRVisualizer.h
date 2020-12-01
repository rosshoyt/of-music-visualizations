#pragma once
#include "Utils.h"
#include "MIDIAnimationComponent.h"
#include "Envelope.h"
#include "spline.h"
#include "ofMain.h"
#include "ofUtils.h"
#include <map>
#include <algorithm>
#include "ofxGui.h"


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
	
	Envelope* envelopeADR;
	Envelope* envelopeADSR;
	EnvelopeNode* envelopeADSRNode;

	// note test params
	uint64_t lastNoteOn = 0;
	uint64_t lastNoteRelease = 0;


	// display params
	//uint64_t colorTimeMS = 100; // in ms
	uint64_t lastColorChange = 0;

	int numPoints = 128;

	std::vector<ofColor> colors;
	ofColor currentColor;

	ofParameter<bool> showADRToggle;
	ofParameter<double> circleSize;
	ofParameter<int> changeSpeed;
	ofParameter<int> adsrTestNoteSpeed;
	//int minYToChange = 1, maxYToChange = scYDefaults.size() - 2, currentYToChange = minYToChange;
	
	// private methods
	void updateTimeBasedParams(uint64_t nowMS);

};