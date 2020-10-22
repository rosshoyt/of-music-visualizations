#pragma once
#include "AnimationComponent.h"
#include "spline.h"
#include "ADSR.h"
#include "ofMain.h"
#include "MIDIPortState.h"
#include "ofUtils.h"
#include <map>

class ADSRVisualizer : public AnimationComponent
{
public:
	ADSRVisualizer(MIDIPortState* midiPortState, std::string uid = "ADSR Visualizer") : AnimationComponent(midiPortState, uid) {

	}
	// method which should be called to initialize the AnimationComponent and prepare it to display at some point in the future.
	// memory allocations, expensive 1-time  initializations and file operations are examples that should be implemented in setup()
	void setup() {


		double midpointLevel = adsr.aL * .2f; // splineAmount;
		
		splineAttack.set_points({ -.5f, 0.f, .5f,           1.f,  1.5f },
			                    { 0.f,  0.f, midpointLevel, adsr.aL, adsr.aL }
		);

		//splineDecay.set_points({ adsr.a, adsr.a + adsr.d / 2.f, adsr.dTot },
		//	{ adsr.aL, 1.f - (1.f - splineAmount) * adsr.dL, adsr.dL }
		//);
		//splineRelease.set_points({ adsr.sTot, adsr.sTot + adsr.r / 2.f, adsr.rTot },
		//	{ adsr.sL, adsr.sL / 2.f, 0.f }//(1.f - splineAmount) * adsr.r, adsr.}
		//);

		//setupColors
		for (int i = 0; i < 32; i++) {
			
			colors.push_back(ofColor((int)ofRandom(0, 255), (int)ofRandom(0, 255), (int)ofRandom(0, 255)));
		}
	}

	ADSR adsr;
	double splineAmount;
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

	void update() {
	
	}

	void draw() {
		float width = ofGetWidth(), height = ofGetHeight();
		for (int i = 0; i < numPoints; ++i) {
			ofSetColor(colors[i % colors.size()]);

			double xF = float(i) / numPoints;
			float heightScale = splineAttack(xF);
			
			ofDrawCircle( { width * xF, height - height * heightScale }, 5);

			//std::cout << "Height Scale = " << heightScale << " for x = " << i << '\n';
		}
	}

	void drawGUI() {

	}

	void drawBackground() {
	}

};

