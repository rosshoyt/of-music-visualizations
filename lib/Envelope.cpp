#include "Envelope.h"

EnvelopeSegment::EnvelopeSegment(EnvelopeSegmentSettings settings) : settings(settings) {
	init();
}

float EnvelopeSegment::getLevel(double timeSinceNoteStart) {
	std::vector<double> xTemps(splineControlX), yTemps = getSplineYControlsWithIntensity();
	tk::spline spline;
	spline.set_points(xTemps, yTemps);
	return spline(timeSinceNoteStart);
}

float EnvelopeSegment::getLevelForRelativeTime(double timeSinceStartOfThisEnvelopeSegment) {
	if (timeSinceStartOfThisEnvelopeSegment >= 0 && timeSinceStartOfThisEnvelopeSegment < totalLength) {
		std::vector<double> xTemps(splineControlXRelative), yTemps = getSplineYControlsWithIntensity();
		tk::spline spline;
		spline.set_points(xTemps, yTemps);
		return spline(timeSinceStartOfThisEnvelopeSegment);
	}
	return 0;
}

double EnvelopeSegment::getLength() {
	return totalLength;
}

float EnvelopeSegment::getStartingLevel() {
	return settings.startLevel;
}

bool EnvelopeSegment::containsTime(double elapsedTimeMS) {
	return elapsedTimeMS >= settings.start && elapsedTimeMS < settings.end;
}

void EnvelopeSegment::init() {
	// set derived fields
	totalLength = settings.end - settings.start;

	// setup 6-point control spline in default state (linear)
	double xStepSize = totalLength / NUM_INTERNAL_CONTROL_SEGMENTS;
	double yStepSize = (settings.endLevel - settings.startLevel) / NUM_INTERNAL_CONTROL_SEGMENTS;
	double yVal = settings.startLevel - yStepSize;
	double xVal = settings.start - xStepSize;
	for (int i = 0; i < 6; ++i, xVal += xStepSize, yVal += yStepSize) {
		splineControlY.push_back(yVal);
		splineControlX.push_back(xVal);
		splineControlXRelative.push_back(xVal - settings.start);
		//std::cout << "Xval = "<< xVal << " Yval = " << yVal << " Xval (relative) = " << splineControlXRelative[i] <<'\n';
	}
	
	// setup intensity slider
	splineIntensitySlider.set("Spline Intensity", 0, -yStepSize * MAX_SPLINE_CONTROL_PERC, yStepSize * MAX_SPLINE_CONTROL_PERC);
}

std::vector<double> EnvelopeSegment::getSplineYControlsWithIntensity() {
	std::vector<double> yTemps(splineControlY);
	yTemps[0] -= splineIntensitySlider;
	yTemps[2] += splineIntensitySlider;
	yTemps[3] += splineIntensitySlider;
	yTemps[5] -= splineIntensitySlider;
	return yTemps;
}

Envelope::Envelope(EnvelopeSettings envelopeSettings) : envelopeSettings(envelopeSettings) {
	init();
}

Envelope::Envelope() : Envelope(EnvelopeSettings()) {
	init();
}

EnvelopeType Envelope::getEnvelopeType() {
	return envelopeSettings.envelopeType;
}

//float Envelope::getLevel(double timeSinceNoteStart, double timeSinceNoteOff) {
//	//int counter = 0;
//	float level = 0;
//	if (envelopeSettings.envelopeType == ADR) {
//		if (timeSinceNoteStart < totalLength) {
//			for (auto& segment : envelopeSegments) {
//				if (segment->containsTime(timeSinceNoteStart)) {
//					level = segment->getLevel(timeSinceNoteStart);
//				}
//			}
//		}
//	}
//	else { // ADSR
//		bool sustained = timeSinceNoteStart < timeSinceNoteOff;
//		// Check Attack
//		if (envelopeSegments[0]->containsTime(timeSinceNoteStart))
//			level = envelopeSegments[0]->getLevel(timeSinceNoteStart);
//		// Check Decay
//		else if (envelopeSegments[1]->containsTime(timeSinceNoteStart))
//			level = envelopeSegments[1]->getLevel(timeSinceNoteStart);
//		// Note is either sustained, in the Release segment, or finished
//		else if (sustained) { 
//			// note is Sustaining - value to return is start level of release curve
//			level = envelopeSegments[2]->getStartingLevel();
//		}
//		else {
//			// note is Releasing
//			level = envelopeSegments[2]->getLevelForRelativeTime(timeSinceNoteOff);
//		}
//	}
//	return level;
//}
//
//float Envelope::getLevel(long timeSinceNoteStart, long timeSinceNoteOff) {
//	return getLevel(double(timeSinceNoteStart), double(timeSinceNoteOff));
//}

double Envelope::getLength() {
	return getAttackLength() + getDecayLength() + getReleaseLength();
}

double Envelope::getAttackLength() {
	return lengthA;//envelopeSegments[0]->getLength();
}

double Envelope::getDecayLength() {
	return lengthD; //envelopeSegments[1]->getLength();
}

double Envelope::getReleaseLength() {
	return lengthR;//totalLength;//envelopeSegments.back()->getLength();
}

void Envelope::init() {
	// init all ofParameters
	sustain.set("Sustain", sustain);

	lengthA.set("Length", lengthA, 0, MAX_SEG_LENGTH);
	levelA.set("Level", levelA, 0, 1);
	splineA.set("Spline", splineA, 0, 1);

	lengthD.set("Length", lengthD, 0, MAX_SEG_LENGTH);
	levelD.set("Level", levelD, 0, 1);
	splineD.set("Spline", splineD, 0, 1);

	lengthR.set("Length", lengthR, 0, MAX_SEG_LENGTH);
	splineR.set("Spline", splineR, 0, 1);


	// TODO move to setupSubGUI()
	guiParams.setName("Envelope Settings");
	attackParams.setName("Attack Parameters");
	releaseParams.setName("Release Parameters");
	decayParams.setName("Decay Parameters");

	attackParams.add(lengthA);
	attackParams.add(levelA);
	attackParams.add(splineA);
	decayParams.add(lengthD);
	decayParams.add(levelD);
	decayParams.add(splineD);
	decayParams.add(lengthR);
	decayParams.add(splineR);
	releaseParams.add(lengthR);
	releaseParams.add(splineR);

	guiParams.add(sustain);
	guiParams.add(attackParams);
	guiParams.add(decayParams);
	guiParams.add(releaseParams);

	






	//int size = envelopeSettings.envSegmentLengths.size();
	//std::cout << "Intializing Envelope with " << size << " env segment lengths\n";
	//for (int i = 0; i < size; ++i) {
	//	// TODO refactor - Envelope Settings could generate a list of EnvelopeSegmentSettings on init()
	//	// Create the segment settings
	//	EnvelopeSegmentSettings segmentSettings;
	//	// Set the start time of the envelopeADR - how much time has passed to this point
	//	segmentSettings.start = totalLength; 
	//	// Set the end time of the envelopeADR
	//	segmentSettings.end = totalLength + envelopeSettings.envSegmentLengths[i];
	//	
	//	// Set the level the envelopeADR should start at
	//	segmentSettings.startLevel = envelopeSettings.envSegmentLevels[i];
	//	// Set the end level
	//	if (i < size - 1) {
	//		//nextLength = envelopeSettings.envSegmentLengths[i+1];
	//		segmentSettings.endLevel = envelopeSettings.envSegmentLevels[i+1];
	//	}
	//	else { // Release envelopeADR, so set end level to 0
	//		segmentSettings.endLevel = 0;
	//	}

	//	EnvelopeSegment* envelopeSegment = new EnvelopeSegment(segmentSettings);
	//	guiParams.add(envelopeSegment->splineIntensitySlider);
	//	envelopeSegments.push_back(envelopeSegment);
	//	
	//	totalLength += envelopeSegment->getLength();
	//}




	std::cout << "initialized envelope with length " << getLength() << '\n';

}

EnvelopeNode::EnvelopeNode(Envelope* envelope) : envelope(envelope) {}

float EnvelopeNode::getLevel(long currentTimeMS) {
	return envelope->getLevel(currentTimeMS - lastStart, currentTimeMS - lastStop);
}

float EnvelopeNode::getLevel() {
	return getLevel(ofGetSystemTimeMillis());
}

void EnvelopeNode::start(int velocity) {
	lastNoteOnVelocity = velocity;
	start();
}

void EnvelopeNode::start() {
	// TODO
	// deal with what happens when start() called before stop() cancels previous note
	// stop();
	lastStart = ofGetSystemTimeMillis();
	//std:cout << "Starting Node at " << lastStart << ", time since last note released = " << lastStart - lastStop <<"\n";
}

void EnvelopeNode::stop() {
	lastStop = ofGetSystemTimeMillis();
	//std:cout << "Stopping Node at " << lastStop << ", length held = " << lastStop - lastStart << "\n";
}

long EnvelopeNode::getLastStartTimeMS() {
	return lastStart;
}

long EnvelopeNode::getLastStopTimeMS() {
	return lastStop;
}

int EnvelopeNode::getLastNoteOnVelocity() {
	return lastNoteOnVelocity;
}
