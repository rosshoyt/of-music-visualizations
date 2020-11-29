#pragma once
#include "Utils.h"
#include "MIDIAnimationComponent.h"
#include "spline.h"
#include "ofMain.h"
#include "ofUtils.h"
#include <map>
#include <algorithm>
#include "ofxGui.h"
#include "ADSR.h"


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

	// private mehotds
	bool shouldChange();

	void drawADSRTest();

	// private fields
	NoteADSRState testADSR;

	//ofParameter<bool> testADSRToggle;
	ofParameter<float> splineIntensitySlider;

	
	tk::spline splineAttack, splineDecay, splineRelease; // TODO unique spline for each ADSR segment

	int numPoints = 128;

	std::vector<ofColor> colors;
	
	const double minF = -2.f;
	const double maxF = 2.f;

	std::vector<ofParameter<double>> splineControlX, splineControlY;
	std::vector<double> scXDefaults 
		//{ 0, .333, .666, 1 };
		{ -.333f, 0.f, .333f, .666f, 1.f, 1.333f };//,  1.5f };
	std::vector<double> scYDefaults 
		//{ 0, .333, .666, 1 }; 
		{ -.333f, 0.f, .333f, .666f, 1.f, 1.333f };//,  1.f };

	uint64_t colorTimeMS = 100; // in ms
	uint64_t lastColorChange = 0;

	ofColor currentColor;

	// display params
	ofParameter<double> circleSize;
	ofParameter<int> changeSpeed;


	int minYToChange = 1, maxYToChange = scYDefaults.size() - 2, currentYToChange = minYToChange;


};


// TODO 
//class EnvelopeParam {
//public:
//
//	EnvelopeParam(double startLevel, double endLevel, std::vector<double> xControlPoints, std::vector<double> yControlPoints) : startLevel(startLevel), endLevel(endLevel), xControlPoints(xControlPoints), yControlPoints(yControlPoints)  {
//
//	}
//
//	EnvelopeParam() {
//
//	}
//
//	double startLevel, endLevel;
//	std::vector<double> xControlPoints, yControlPoints;
//	
//	//EnvelopeParam attack, decay, sustain, release;
//
//
//
//	ADSR adsr;
//	std::vector<ofParameter<double>> xPoints { -.5f, 0.f, .5f, 1.f,  1.5f };// ,
//		//yPoints{ 0.f,  0.f, midpointLevel, adsr.aL, adsr.aL}
//	
//	/*void setup(ofxPanel& panel) {
//		panel.add(textEntryField.setup());
//	}*/
//};