#include "ADSRVisualizer.h"

//--------------------------------------------------------------
ADSRVisualizer::ADSRVisualizer(std::string uid) : MIDIAnimationComponent(uid) {}

//--------------------------------------------------------------
void ADSRVisualizer::setup() {
	EnvelopeSettings envelopeSettings;
	envelopeSettings.envelopeType = ADR;
	envelopeSettings.envSegmentLengths = { 200, 1600, 200 };
	envelopeSettings.envSegmentLevels  = { 0,      1,  .5 };
	envelope = new Envelope(envelopeSettings);
	
	//setup Colors
	for (int i = 0; i < 32; i++) {
		colors.push_back(utils::color::getRandomColor());
	}
}

//--------------------------------------------------------------
void ADSRVisualizer::setupGUI() {		
	//gui.add(attackSegment.splineIntensitySlider);
	gui.add(envelope->guiParams);
	//gui.add(splineIntensitySlider.set("Spline Intensity", 0, -.3, .3));
	/*for (int i = 0; i < scXDefaults.size(); ++i) {
		auto pNumber = std::to_string(i + 1);
		splineControlX.push_back(ofParameter<double>().set("x" + pNumber, scXDefaults[i], minF, maxF));
		splineControlY.push_back(ofParameter<double>().set("y" + pNumber, scYDefaults[i], minF, maxF));
		gui.add(splineControlX[i]);
		gui.add(splineControlY[i]);
	}*/
	/*gui.add(splineControlX2.set("control point X", .5f, minF, maxF));
	gui.add(splineControlY2.set("spline Amount", .75f, minF, maxF));*/
	gui.add(changeSpeed.set("color speed ms", 300, 1, 5000));
	gui.add(circleSize.set("circle size", 5.f, 0.1f, 100.f));
}

//--------------------------------------------------------------
void ADSRVisualizer::prepare() {}

//--------------------------------------------------------------
void ADSRVisualizer::takedown() {}

//--------------------------------------------------------------
void ADSRVisualizer::update() {}

//--------------------------------------------------------------
bool ADSRVisualizer::shouldChange() {
	//update color
	auto nowMS = ofGetSystemTimeMillis();
	if (nowMS > lastColorChange + changeSpeed) {
		lastColorChange = nowMS;
		return true;
	}
	return false;
}

//--------------------------------------------------------------
void ADSRVisualizer::draw() {
	/*int maxVel = 0;
	int numNotes = 0;
	for (auto channel : midiPortState->getAllChannelNotes()) {
		for (auto note : channel) {
			maxVel = std::max(note.second, maxVel);
			++numNotes;
		}
	}*/

	//update time-based params
	if (shouldChange()) {
		ofSetColor(utils::color::getRandomColor()); //colors[i % colors.size()]);
		// update which y val to augment
		/*if (currentYToChange > maxYToChange) {
			currentYToChange = minYToChange;
		}
		else {
			++currentYToChange;
		}*/
	}

	for (int i = 0; i < numPoints; ++i) {

		double xF = float(i) / numPoints;
		float heightScale = envelope->getLevel(xF * envelope->getLength());
		
		auto circleX = animationWidth * xF;
		auto circleY = animationHeight - animationHeight * heightScale ;
		//std::cout << "X: " << circleX << " Y:"<< circleY << " xF = " << xF << " Level =" << heightScale << '\n';

		ofDrawCircle({ circleX , circleY }, circleSize);

		//std::cout << "Height Scale = " << heightScale << " for x = " << i << '\n';
	}

}