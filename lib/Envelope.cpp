#include "Envelope.h"

Envelope::Envelope() {
	init();
}

float Envelope::getLevel(long timeSinceStart, long timeSinceEnd, float lastNoteLevel) {
	std::string debug = "";

	double level = 0;

	// check if note is currently engaged.
	bool heldNote = timeSinceEnd > timeSinceStart;

	bool heldNoteButMayBeReleasing = false;
	if (heldNote) {
		long timeThreshold = lengthA;

		if (timeThreshold > timeSinceStart) {
			// attacking 
			level = getLevelFromSegment(timeSinceStart, lengthA, 0, levelA, splineA);
			debug = "Attacking";
		}
		else if ((timeThreshold += lengthD) > timeSinceStart) {
			// decaying
			level = getLevelFromSegment(timeSinceStart - lengthA, lengthD, levelA, levelD, splineD);
			debug = "Decaying";
		}
		else if (sustain) {
			// sustaining
			level = levelD;//getLevel(timeSinceStart - lengthA - lengthD, st)
			debug = "Sustaining";
		}
		else {
			// timePastRelease = timeSinceStart - timeThreshold;
			//heldNoteButMayBeReleasing = true;
			debug = "Held down";
			auto elapsedSinceEndOfDecay = timeSinceStart - timeThreshold;
			if (elapsedSinceEndOfDecay < lengthR) {
				// releasing
				level = getLevelFromSegment(elapsedSinceEndOfDecay, lengthR, levelD, 0, splineR);
				debug.append(" but in Release curve");
			}
			else {
				debug.append(" but envelope completed.");
			}
		}
	}
	else {
		// note is not being held down right now
		debug = "Not held down";
		if (timeSinceEnd < lengthR) {
			level = getLevelFromSegment(timeSinceEnd, lengthR, lastNoteLevel, 0, splineR);
			debug.append(" and in Release Curve");
		}
	}
	debug = debug == "" ? "Inactive" : debug;
	//std::cout << "Time Since Start: " << timeSinceStart << " State: " << debug << " Value = " << level << std::endl;
	return level;
}

float getLevel(double segTimeElapsed, double lengthSeg, double levelSegStart, double levelSegEnd, double levelSpline) {
	float level;
	if (levelSegStart == 0 && levelSegEnd == 0) {
		level = 0;
	}
	else {
		std::vector<double> splineControlX, splineControlY;

		//int numInternalControlSegments = 3; // TODO use named variable instead of 'magic number'

		// setup 6-point control spline in default state (linear)
		double xStepSize = lengthSeg / 3;
		double yStepSize = (levelSegEnd - levelSegStart) / 3;
		double yVal = levelSegStart - yStepSize;
		double xVal = -xStepSize;//settings.start - xStepSize;
		for (int i = 0; i < 6; ++i, xVal += xStepSize, yVal += yStepSize) {
			splineControlY.push_back(yVal);
			splineControlX.push_back(xVal);
			//std::cout << "Xval = "<< xVal << " Yval = " << yVal << " Xval (relative) = " << splineControlXRelative[i] <<'\n';
		}

		if (levelSpline != 0) {
			// TODO change the direction of the control points depending on segment slope being positive, negative (or 0?)
			// double change = levelSegStart > levelSegEnd ? -change : change;
			// TODO also add error correction so that spline values are not toop big/small
			splineControlY[0] -= levelSpline;
			splineControlY[2] += levelSpline;
			splineControlY[3] += levelSpline;
			splineControlY[5] -= levelSpline;
		} // else { linear interpolate to save processing power? }; // TODO

		tk::spline spline;

		spline.set_points(splineControlX, splineControlY);


		level = spline(segTimeElapsed);
	}
	return level;
}

float Envelope::getLevelFromSegment(double segTimeElapsed, double lengthSeg, double levelSegStart, double levelSegEnd, double levelSpline) {
		float level;
		if (levelSegStart == 0 && levelSegEnd == 0) {
			level = 0;
		}
		else{
			std::vector<double> splineControlX, splineControlY;

			//int numInternalControlSegments = 3; // TODO use named variable instead of 'magic number'

			// setup 6-point control spline in default state (linear)
			double xStepSize = lengthSeg / 3;
			double yStepSize = (levelSegEnd - levelSegStart) / 3;
			double yVal = levelSegStart - yStepSize;
			double xVal = -xStepSize;//settings.start - xStepSize;
			for (int i = 0; i < 6; ++i, xVal += xStepSize, yVal += yStepSize) {
				splineControlY.push_back(yVal);
				splineControlX.push_back(xVal);
				//std::cout << "Xval = "<< xVal << " Yval = " << yVal << " Xval (relative) = " << splineControlXRelative[i] <<'\n';
			}

			if (levelSpline != 0) {
				// TODO change the direction of the control points depending on segment slope being positive, negative (or 0?)
				// double change = levelSegStart > levelSegEnd ? -change : change;
				// TODO also add error correction so that spline values are not toop big/small
				splineControlY[0] -= levelSpline;
				splineControlY[2] += levelSpline;
				splineControlY[3] += levelSpline;
				splineControlY[5] -= levelSpline;
			} // else { linear interpolate to save processing power? }; // TODO

			tk::spline spline;

			spline.set_points(splineControlX, splineControlY);


			level = spline(segTimeElapsed);
		}
		return level;
	}
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
	// set names of groups
	guiParams.setName("Envelope Settings");
	attackParams.setName("Attack Parameters");
	releaseParams.setName("Release Parameters");
	decayParams.setName("Decay Parameters");

	// add params to the param groups
	attackParams.add(lengthA);
	attackParams.add(levelA);
	attackParams.add(splineA);
	
	decayParams.add(lengthD);
	decayParams.add(levelD);
	decayParams.add(splineD);
	
	releaseParams.add(lengthR);
	releaseParams.add(splineR);

	// add sub-param groups to main group
	guiParams.add(sustain);
	guiParams.add(attackParams);
	guiParams.add(decayParams);
	guiParams.add(releaseParams);
	std::cout << "initialized envelope with length " << getLength() << '\n';

}

EnvelopeNode::EnvelopeNode(Envelope* envelope) : envelope(envelope) {}

float EnvelopeNode::getLevel(long currentTimeMS) {
	auto level = envelope->getLevel(currentTimeMS - lastStart, currentTimeMS - lastStop, lastLevel);
	if (lastStart > lastStop)
		lastLevel = level;
	return level;
	//return envelope->getLevel(currentTimeMS - lastStart, currentTimeMS - lastStop, lastLevel);

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
