#include "Envelope.h"

//EnvelopeSegment::EnvelopeSegment(double start, double startLevel, double end, double endLevel) : type(type), start(start), startLevel(startLevel), end(end), endLevel(endLevel) {
//	init();
//}

// TODO refactor - store calculated value to avoid calculating redundantly?

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
		std::cout << "Xval = "<< xVal << " Yval = " << yVal << " Xval (relative) = " << splineControlXRelative[i] <<'\n';
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

float Envelope::getLevel(double timeSinceNoteStart, double timeSinceNoteOff) {
	//int counter = 0;
	float level = 0;
	
	
	
	if (envelopeSettings.envelopeType == ADR) {
		if (timeSinceNoteStart < totalLength) {
			for (auto& segment : envelopeSegments) {
				if (segment->containsTime(timeSinceNoteStart)) {
					//std::cout << "Getting Level from segment # " << counter + 1 << '\n';
					level = segment->getLevel(timeSinceNoteStart);
				}
				//++counter;
			}
		}
	}
	else { // ADSR
		bool sustained = timeSinceNoteStart < timeSinceNoteOff;
		//std::cout << "Evauluating ADSR with TimeSinceNoteStart = " << timeSinceNoteStart << ", TimeSinceNoteOFF = " << timeSinceNoteOff << " Sustained: " << std::boolalpha << sustained << '\n';


		// Check Attack
		if (envelopeSegments[0]->containsTime(timeSinceNoteStart))
			level = envelopeSegments[0]->getLevel(timeSinceNoteStart);
		// Check Decay
		else if (envelopeSegments[1]->containsTime(timeSinceNoteStart))
			level = envelopeSegments[1]->getLevel(timeSinceNoteStart);
		// Note is either sustained, in the Release segment, or finished
		else if (sustained) { 
			// note is Sustaining - value to return is start level of release curve
			level = envelopeSegments[2]->getStartingLevel();
		}
		else { //if (envelopeSegments[2]->containsTime(timeSinceNoteStart)) { 
			// note is Releasing
			
			//level = envelopeSegments[2]->getLevel(timeSinceNoteStart);
			level = envelopeSegments[2]->getLevelForRelativeTime(timeSinceNoteOff);
		}


		   //for (int i = 0; i < envelopeSegments.size(); ++i) {
		//	//if()




		//	if (i < 2 && envelopeSegments[i]->containsTime(timeSinceNoteStart)) {
		//		
		//		level = envelopeSegments[i]->getLevel(timeSinceNoteStart);
		//		//std::cout << "Found level in Attack/Decay" << 
		//	}
		//	else {
		//		if (sustained) {
		//			level = envelopeSegments[i]->getStartingLevel();
		//		}
		//		else if (envelopeSegments[i]->containsTime(timeSinceNoteStart)) {
		//			level = envelopeSegments[i]->getLevel(timeSinceNoteStart);
		//		}
		//	}

		//}
		   		//	level = envelopeSegments
	}
	return level;
}

// TODO Refactor (could cause feedback loop if casting doesn't go correctly
float Envelope::getLevel(long timeSinceNoteStart, long timeSinceNoteOff) {
	return getLevel(double(timeSinceNoteStart), double(timeSinceNoteOff));
}

double Envelope::getLength() {
	return totalLength;
}

void Envelope::init() {
	// initialize the segments of the envelopeADR
	totalLength = 0;
	int size = envelopeSettings.envSegmentLengths.size();
	for (int i = 0; i < size; ++i) {
		// TODO refactor - Envelope Settings could generate a list of EnvelopeSegmentSettings on init()
		// Create the segment settings
		EnvelopeSegmentSettings segmentSettings;
		// Set the start time of the envelopeADR - how much time has passed to this point
		segmentSettings.start = totalLength; 
		// Set the end time of the envelopeADR
		segmentSettings.end = totalLength + envelopeSettings.envSegmentLengths[i];
		
		// Set the level the envelopeADR should start at
		segmentSettings.startLevel = envelopeSettings.envSegmentLevels[i];
		// Set the end level
		if (i < size - 1) {
			//nextLength = envelopeSettings.envSegmentLengths[i+1];
			segmentSettings.endLevel = envelopeSettings.envSegmentLevels[i+1];
		}
		else { // Release envelopeADR, so set end level to 0
			segmentSettings.endLevel = 0;
		}

		EnvelopeSegment* envelopeSegment = new EnvelopeSegment(segmentSettings);
		guiParams.add(envelopeSegment->splineIntensitySlider);
		envelopeSegments.push_back(envelopeSegment);
		
		
		
		totalLength += envelopeSegment->getLength();
	}

	std::cout << "initialized envelope with size " << totalLength << '\n';

}
