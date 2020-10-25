#pragma once
#include "AnimationComponent.h"
#include "spline.h"
#include "ADSR.h"
#include "ofMain.h"
#include "MIDIPortState.h"
#include "ofUtils.h"
#include <map>
#include <algorithm>
#include "ofxGui.h"



class ADSRVisualizer : public AnimationComponent {
public:
	ADSRVisualizer(MIDIPortState* midiPortState, std::string uid = "ADSR Visualizer") : AnimationComponent(midiPortState, uid) {

	}

	ADSR adsr;

	double minF = -2.f;
	double maxF = 2.f;

	std::vector<ofParameter<double>> splineControlX, splineControlY;
	std::vector<double> scXDefaults { -.5f, 0.f, .333f, .5f,.666f, 1.f,  1.5f };
	std::vector<double> scYDefaults { 0.f,  0.f, .25f, .5f, .75f, 1.f, 1.f };

	// display params
	ofParameter<double> circleSize;
	ofParameter<int> changeSpeed;
	int minYToChange = 1, maxYToChange = scYDefaults.size() - 2, currentYToChange = minYToChange;

	void setup() {

		gui.setup();
		//gui.add(textEntryField.setup("control point X", .75f));
		

		for (int i = 0; i < scXDefaults.size(); ++i) {
			auto pNumber = std::to_string(i + 1);
			splineControlX.push_back(ofParameter<double>().set("x" + pNumber, scXDefaults[i], minF, maxF));
			splineControlY.push_back(ofParameter<double>().set("y" + pNumber, scYDefaults[i], minF, maxF));

			gui.add(splineControlX[i]);
			gui.add(splineControlY[i]);
		}


		/*gui.add(splineControlX2.set("control point X", .5f, minF, maxF));
		gui.add(splineControlY2.set("spline Amount", .75f, minF, maxF));*/
		
		gui.add(changeSpeed.set("color speed ms", 300, 1, 5000));
		gui.add(circleSize.set("circle size", 5.f, 0.1f, 100.f));		
		

		//setupColors
		for (int i = 0; i < 32; i++) {
			
			colors.push_back(getRandomColor());
		}
	}

	ofColor getRandomColor() {
		return ofColor((int)ofRandom(0, 255), (int)ofRandom(0, 255), (int)ofRandom(0, 255));
	}

	ofxPanel gui;
	//double splineControlY2;
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

	bool shouldChange() {
		//update color
		auto nowMS = ofGetSystemTimeMillis();
		if (nowMS > lastColorChange + changeSpeed) {
			lastColorChange = nowMS;
			return true;
		}
		return false;
	}
	void draw() {
		
		gui.draw();
		int maxVel = 0;
		int numNotes = 0;
		for (auto channel : midiPortState->getAllChannelNotes()) {
			for (auto note : channel) {
				maxVel = std::max(note.second, maxVel);
				++numNotes;
			}
		}

		tk::spline spline;
		std::vector<double> xTemps, yTemps;

		//update time-based params
		if (shouldChange()) {
			ofSetColor(getRandomColor()); //colors[i % colors.size()]);
			// update which y val to augment
			if (currentYToChange > maxYToChange) {
				currentYToChange = minYToChange;
			}
			else {
				++currentYToChange;
			}

		}

		auto size = splineControlX.size();
		for (auto i = 0; i < size; i++) {
			//if(i < size - 1)
			xTemps.push_back(splineControlX[i]);//a1//i < size - 1 ? std::min(splineControlX[i].get(),splineControlX[i+1].get()) : splineControlX[i]);
			yTemps.push_back(i == currentYToChange && numNotes > 0 ? splineControlY[i] * maxVel / 128.f : splineControlY[i]); 
		}
		
		//std::sort(xTemps.begin(), xTemps.end());//, sortDouble);

		spline.set_points(xTemps, yTemps);
		/*spline.set_points({ scXDefaults[0], splineControlX[1], splineControlX[2], splineControlX[3], scXDefaults[4] },
			              { splineControlY[0], splineControlY[1], splineControlY[2], splineControlY[3], splineControlY[4] }
		);
		*/
		float width = ofGetWidth(), height = ofGetHeight();

		
		
		for (int i = 0; i < numPoints; ++i) {
			
			
			

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

