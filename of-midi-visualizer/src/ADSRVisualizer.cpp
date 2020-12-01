#include "ADSRVisualizer.h"

//--------------------------------------------------------------
ADSRVisualizer::ADSRVisualizer(std::string uid) : MIDIAnimationComponent(uid) {}

//--------------------------------------------------------------
void ADSRVisualizer::setup() {
	// Create ADR envelope
	EnvelopeSettings envelopeSettingsADR;
	envelopeSettingsADR.envelopeType = ADR;
	envelopeSettingsADR.envSegmentLengths = { 400, 1400, 400 };
	envelopeSettingsADR.envSegmentLevels  = { 0,      1,  .5 };
	envelopeADR = new Envelope(envelopeSettingsADR);
	
	// Create ADSR envelope
	EnvelopeSettings envelopeSettingsADSR(envelopeSettingsADR);
	envelopeSettingsADSR.envelopeType = ADSR;
	envelopeADSR = new Envelope(envelopeSettingsADSR);
	// Setup ADSR Envelope Node
	envelopeADSRNode = new EnvelopeNode(envelopeADSR);


	//setup Colors
	for (int i = 0; i < 32; i++) {
		colors.push_back(utils::color::getRandomColor());
	}
}

//--------------------------------------------------------------
void ADSRVisualizer::setupGUI() {		
	//gui.add(attackSegment.splineIntensitySlider);
	gui.add(showADRToggle.set("Show ADR", false));
	gui.add(adsrTestNoteSpeed.set("Test Note Length MS", 4000, 1, 20000));
	envelopeADSR->guiParams.setName("ADSR Params");
	envelopeADR->guiParams.setName("ADR Params");
	gui.add(envelopeADSR->guiParams);
	gui.add(envelopeADR->guiParams);
	
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
void ADSRVisualizer::draw() {
	auto timeNowMS = ofGetSystemTimeMillis();
	updateTimeBasedParams(timeNowMS);

	float heightScale, circleX, circleY;

	if (!showADRToggle) {
		// Draw the ADSR visualizer as a horizontal line, 
		// so we'll get the level now instead of per-point
		heightScale = envelopeADSRNode->getLevel(timeNowMS);
	}

	for (int i = 0; i < numPoints; ++i) {

		double xF = float(i) / numPoints;
		circleX = animationWidth * xF;

		if (showADRToggle) { //Draw the ADR visualizer, so get a height value for current point
			heightScale = envelopeADR->getLevel(xF * envelopeADR->getLength(), true);
		}
		
		circleY = animationHeight - animationHeight * heightScale;
		ofDrawCircle({ circleX , circleY }, circleSize);
	}

	// debug TODO delete
	if (!showADRToggle) {
		std::cout << "ADSR Y = "<< circleY << ", Height Scale (Level) = " << heightScale << '\n';

	}
}

//--------------------------------------------------------------
void ADSRVisualizer::updateTimeBasedParams(uint64_t nowMS) {
	//auto nowMS = ofGetSystemTimeMillis();
	// update the ADSR Node (simulating note being played to test ADSR)
	double totalEnvLen = adsrTestNoteSpeed + envelopeADSR->getReleaseLength();

	// TODO refactor - use EnvelopeNode's getLastStart(), getLastStop()
	if (nowMS > lastNoteOn + totalEnvLen) { //Note needs to start
		lastNoteOn = nowMS;
		envelopeADSRNode->start();
	}
	else if (nowMS > lastNoteOn + adsrTestNoteSpeed && lastNoteOn > lastNoteRelease) { //Note needs to release
		lastNoteRelease = nowMS;
		envelopeADSRNode->stop();
	}
	
	
	//update color
	if (nowMS > lastColorChange + changeSpeed) {
		lastColorChange = nowMS;
		ofSetColor(utils::color::getRandomColor());
	}



	
}
