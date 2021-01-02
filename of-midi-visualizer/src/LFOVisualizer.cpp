#include "LFOVisualizer.h"

LFOVisualizer::LFOVisualizer(std::string uid) : MIDIAnimationGUIComponent(uid), sineLFO() {}

void LFOVisualizer::setupGUI() {
	gui.add(lfoTimeLength.set("LFO Time Length (MS)", lfoTimeLength, 4, 1000));
	gui.add(numPoints.set("Number of Points", numPoints, 1 << 5, 1 << 12));
	gui.add(circleSize.set("Circle Size", circleSize, 25, 110));
	gui.add(pointLength.set("Point Length (MS)", pointLength, 10, 100));
	gui.add(color.set("LFO Visualizer Color", color));
}

void LFOVisualizer::setupParameterListeners() {
	addParameterListener(lfoTimeLength);
	addParameterListener(numPoints);
	addParameterListener(pointLength);
	addParameterListener(circleSize);
	addParameterListener(color);
}


void LFOVisualizer::setup() {}

void LFOVisualizer::update() {}

void LFOVisualizer::draw() {
	ofSetColor(color);
	sineLFO.setTimeLength(lfoTimeLength);
	for (int i = 0; i < numPoints; ++i) {
		float x = float(i) / float(numPoints);
		float y = sineLFO.getValue(i * pointLength);
		ofDrawCircle({ x * animationWidth, y * animationHeight}, circleSize);
	}
}
