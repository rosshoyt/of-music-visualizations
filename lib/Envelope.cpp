#include "Envelope.h"

//EnvelopeSegment::EnvelopeSegment(double start, double startLevel, double end, double endLevel) : type(type), start(start), startLevel(startLevel), end(end), endLevel(endLevel) {
//	init();
//}

// TODO refactor - store calculated value to avoid calculating redundantly?

EnvelopeSegment::EnvelopeSegment(EnvelopeSegmentSettings settings) : settings(settings) {
	init();
}

float EnvelopeSegment::getLevel(double xVal) {

	std::vector<double> xTemps(splineControlX), yTemps(splineControlY);

	yTemps[0] -= splineIntensitySlider;
	yTemps[2] += splineIntensitySlider;
	yTemps[3] += splineIntensitySlider;
	yTemps[5] -= splineIntensitySlider;

	tk::spline spline;
	spline.set_points(xTemps, yTemps);

	return spline(xVal);
}

double EnvelopeSegment::getLength() {
	return totalLength;
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
		std::cout << "Xval = "<< xVal << " Yval = " << yVal << '\n';
	}
	
	// setup intensity slider
	splineIntensitySlider.set("Spline Intensity", 0, -yStepSize * MAX_SPLINE_CONTROL_PERC, yStepSize * MAX_SPLINE_CONTROL_PERC);
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

float Envelope::getLevel(double time) {
	int counter = 0;
	for (auto& segment : envelopeSegments) {
		if (segment->containsTime(time)) {
			//std::cout << "Getting Level from segment # " << counter + 1 << '\n';
			return segment->getLevel(time);
		}
		++counter;
	}
	return 0;
}

float Envelope::getLevel(long time) {
	return getLevel(double(time));
}

double Envelope::getLength() {
	return totalLength;
}

void Envelope::init() {
	// initialize the segments of the envelope
	totalLength = 0;
	int size = envelopeSettings.envSegmentLengths.size();
	for (int i = 0; i < size; ++i) {
		// TODO refactor - Envelope Settings could generate a list of EnvelopeSegmentSettings on init()
		// Create the segment settings
		EnvelopeSegmentSettings segmentSettings;
		// Set the start time of the envelope - how much time has passed to this point
		segmentSettings.start = totalLength; 
		// Set the end time of the envelope
		segmentSettings.end = totalLength + envelopeSettings.envSegmentLengths[i];
		
		// Set the level the envelope should start at
		segmentSettings.startLevel = envelopeSettings.envSegmentLevels[i];
		// Set the end level
		if (i < size - 1) {
			//nextLength = envelopeSettings.envSegmentLengths[i+1];
			segmentSettings.endLevel = envelopeSettings.envSegmentLevels[i+1];
		}
		else { // Release envelope, so set end level to 0
			segmentSettings.endLevel = 0;
		}

		EnvelopeSegment* envelopeSegment = new EnvelopeSegment(segmentSettings);
		guiParams.add(envelopeSegment->splineIntensitySlider);
		envelopeSegments.push_back(envelopeSegment);
		
		
		
		totalLength += envelopeSegment->getLength();
	}

	std::cout << "initialized envelope with size " << totalLength << '\n';

}
