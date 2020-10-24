#pragma once
#include "AnimationComponent.h"
#include "spline.h"
#include "ADSR.h"
#include "ofMain.h"
#include "MIDIPortState.h"
#include "ofUtils.h"
#include <map>
#include "ofxGui.h"



class ADSRVisualizer : public AnimationComponent {
public:
	ADSRVisualizer(MIDIPortState* midiPortState, std::string uid = "ADSR Visualizer") : AnimationComponent(midiPortState, uid) {

	}

	ADSR adsr;
	ofxInputField<double> textEntryField;
	ofParameter<double> splineControlX, splineControlY;
	ofParameter<double> circleSize;


	void setup() {

		gui.setup();
		gui.add(textEntryField.setup("control point X", .75f));
		gui.add(splineControlX.set("control point X", .5f, 0.000001f, .99999f));
		gui.add(splineControlY.set("spline Amount", .75f, 0.000001, .99999f));
		

		gui.add(circleSize.set("circle size", 5.f, 0.f, 100.f));		
		

		//setupColors
		for (int i = 0; i < 32; i++) {
			
			colors.push_back(getRandomColor());
		}
	}

	ofColor getRandomColor() {
		return ofColor((int)ofRandom(0, 255), (int)ofRandom(0, 255), (int)ofRandom(0, 255));
	}

	ofxPanel gui;
	//double splineControlY;
	tk::spline splineAttack, splineDecay, splineRelease; // TODO unique spline for each ADSR segment

	int numPoints = 128;

	std::vector<ofColor> colors;

	// method which should be called first when the AnimationComponent is changed-to. 
	// for example, when it is selected to be displayed from the dropdown menu
	// sets up any destructive rendering settings which may affect other animations
	void prepare() {}

	// method which should be called first when an animation is being changed-from
	// for exmaple, when it is the current animation and another is selected from the dropdown menu
	void takedown() {}

	void update() {}

	uint64_t colorTimeMS = 100; // in ms
	uint64_t lastColorChange = 0;

	ofColor currentColor;

	void draw() {
		gui.draw();
		tk::spline spline;
		spline.set_points({ -.5f, 0.f, splineControlX, 1.f,  1.5f },
			              { 0.f,  0.f, splineControlY, adsr.aL, adsr.aL }
		);
		

		float width = ofGetWidth(), height = ofGetHeight();

		for (int i = 0; i < numPoints; ++i) {
			//update color
			auto nowMS = ofGetSystemTimeMillis();
			if (nowMS > lastColorChange + colorTimeMS) {
				ofSetColor(getRandomColor()); //colors[i % colors.size()]);
				//currentColor = getRandomColor();
				lastColorChange = nowMS;
			}
			

			double xF = float(i) / numPoints;
			float heightScale = spline(xF);
			
			auto circleX = width * xF;
			auto circleY = height - height * heightScale;
			ofDrawCircle( { circleX , circleY }, circleSize);

			//std::cout << "Height Scale = " << heightScale << " for x = " << i << '\n';
		}
	}

	void drawGUI() {

	}

	void drawBackground() {
	}

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

