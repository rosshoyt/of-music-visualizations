#include "LFOVisualizer.h"

LFOVisualizer::LFOVisualizer(std::string uid) : MIDIAnimationGUIComponent(uid), sineLFO() {}

void LFOVisualizer::setupGUI()
{
}

void LFOVisualizer::setup()
{
	sineLFO.setTimeLength(lfoTimeLength);
}

void LFOVisualizer::update()
{
}

void LFOVisualizer::draw()
{
	int numPoints = 128;
	double pointLength = 10; // each point drawn will equal 10 MS
	float circleSize = 20;
	ofSetColor(ofColor::green);
	for (int i = 0; i < numPoints; ++i) {
		auto x = float(i) / float(numPoints) * animationWidth;
		auto y = sineLFO.getValue(i * pointLength);

		ofDrawCircle({ x, y}, circleSize);


	}
}
