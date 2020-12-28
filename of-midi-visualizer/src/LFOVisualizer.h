#pragma once
#include "MIDIAnimationGUIComponent.h"
#include "LFO.h"

class LFOVisualizer :
	public MIDIAnimationGUIComponent
{
public:
	LFOVisualizer(std::string uid = "LFOVisualizer");
	// Inherited via GUIAnimationComponent
	void setupGUI() override;
	void setup() override;
	void update() override;
	void draw() override;
private:
	SineLFO sineLFO;

	ofParameter<double> lfoTimeLength = 250;
	
};

