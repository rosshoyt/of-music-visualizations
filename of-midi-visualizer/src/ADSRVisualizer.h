#pragma once
#include "MIDIAnimationComponent.h"
#include "spline.h"
#include "ofMain.h"
#include "ofUtils.h"
#include <map>
#include <algorithm>
#include "ofxGui.h"

class ADSRVisualizer : public MIDIAnimationComponent {
public:
	ADSRVisualizer(std::string uid = "ADSR Visualizer");


	double minF = -2.f;
	double maxF = 2.f;

	std::vector<ofParameter<double>> splineControlX, splineControlY;
	std::vector<double> scXDefaults { -.5f, 0.f, .333f, .5f,.666f, 1.f,  1.5f };
	std::vector<double> scYDefaults { 0.f,  0.f, .25f, .5f, .75f, 1.f, 1.f };



	// display params
	ofParameter<double> circleSize;
	ofParameter<int> changeSpeed;
	int minYToChange = 1, maxYToChange = scYDefaults.size() - 2, currentYToChange = minYToChange;

	void setup();

	void setupGUI() override; 

	// TODO move to utils.h
	ofColor getRandomColor();


	//double splineControlY2;
	tk::spline splineAttack, splineDecay, splineRelease; // TODO unique spline for each ADSR segment

	int numPoints = 128;

	std::vector<ofColor> colors;

	// method which should be called first when the AnimationComponent is changed-to. 
	// for example, when it is selected to be displayed from the dropdown menu
	// sets up any destructive rendering settings which may affect other animations
	void prepare();

	// method which should be called first when an animation is being changed-from
	// for exmaple, when it is the current animation and another is selected from the dropdown menu
	void takedown();

	void update();

	uint64_t colorTimeMS = 100; // in ms
	uint64_t lastColorChange = 0;

	ofColor currentColor;

	bool shouldChange();
	void draw();





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