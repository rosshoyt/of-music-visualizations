#pragma once
#include "MIDIAnimationGUIComponent.h"
#include "LFO.h"

class LFOVisualizer : public MIDIAnimationGUIComponent {
public:
	LFOVisualizer(std::string uid = "LFOVisualizer");
	// Inherited via GUIAnimationComponent
	void setupGUI() override;
	void setup() override;
	void setupParameterListeners() override;
	void update() override;
	void draw() override;
private:
	SineLFO sineLFO;

	// gui params
	ofParameter<double> lfoTimeLength = 250;
	ofParameter<int> numPoints = 1024;
	ofParameter<double> circleSize = 20;
	ofParameter<double> pointLength = 10; // each point drawn will equal 10 MS
	ofParameter<ofColor> color = ofColor::mediumSeaGreen;
};

	