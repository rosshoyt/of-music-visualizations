#include "ADSR.h"

ADSR::ADSR(int a, int d, int s, int r, bool sustainLoop) : a(a), d(d), s(s), r(r), aL(1.f), dL(.3f), sL(.0f), sustainLoop(sustainLoop), envelopeSections() {
	init();
	//std::cout << "ADSR total = " << total << '\n';
}

void ADSR::init() {
	aTot = a;
	dTot = a + d;
	sTot = a + d + s;
	rTot = total = a + d + s + r;

	splineControlY = .3f;
}

NoteADSRState::NoteADSRState(ADSR adsr, bool splineCurve, double splineControlY) : adsr(adsr), splineControlY(.65f), adsrState(OFF), active(), startTime(), endTime() {
	initSplines();
}

void NoteADSRState::start() {
	startTime = utils::time::getCurrentTime();
	//active.store(true);
	adsrState = ATTACKING;

}

void NoteADSRState::stop() {
	endTime = utils::time::getCurrentTime();
	//level = 0.f;
	//active.store(false);
	adsrState = OFF;
}

float NoteADSRState::getLevel() {
	// auto level(0.f);
	if (adsrState != OFF) {
		//std::cout << "Getting time elapsed since start time = " << startTime << '\n';
		auto timePassed = utils::time::elapsedTimeSince(startTime);

		auto level = getLevel(timePassed, false);
		//std::cout << std::setw(5) << timePassed << " milliseconds elapsed, note ADSR level = " << level << '\n'; //<< ", regular lerp = "<< getLevel(timePassed, false)<< '\n';
		return level;
	}
	return 0.f;//level;
}

void NoteADSRState::setADSR(ADSR adsr) {
	this->adsr = adsr;
}

void NoteADSRState::initSplines() {

	splineAttack.set_points({ 0.f, adsr.a / 2.f,           adsr.a },
		{ 0.f, splineControlY * adsr.aL, adsr.aL }
	);
	splineDecay.set_points({ adsr.a, adsr.a + adsr.d / 2.f, adsr.dTot },
		{ adsr.aL, 1.f - (1.f - splineControlY) * adsr.dL, adsr.dL }
	);
	splineRelease.set_points({ adsr.sTot, adsr.sTot + adsr.r / 2.f, adsr.rTot },
		{ adsr.sL, adsr.sL / 2.f, 0.f }//(1.f - splineControlY) * adsr.r, adsr.}
	);
}

void NoteADSRState::updateState() {

}

float NoteADSRState::getLevel(long elapsed, bool spline) {
	//if(splineCurve){ // TODO fix splineCurve boolean flag (remove?)

	// segment of the ADSR curve that the elapsed time falls into
	//long currentSegmentTimeLength(adsr.a),  segmentTimeLength = currentSegmentTimeLength;
	long segmentTimeLength;
	// how much of the current ADSR segment has been completed (in time)
	long segmentCompleted;
	// start/end levels of current segment to be determined
	long startLevel, endLevel;

	// Attacking
	if (elapsed <= adsr.aTot) {
		startLevel = 0.f;
		endLevel = adsr.aL;
		segmentTimeLength = adsr.a;
		segmentCompleted = elapsed;

		if (spline)
			return splineAttack(double(segmentCompleted));
	}
	// Decaying
	else if (elapsed <= adsr.dTot) {
		startLevel = adsr.aL;
		endLevel = adsr.dL;
		segmentTimeLength = adsr.d;
		segmentCompleted = elapsed - adsr.aTot;

		if (spline)
			return splineDecay(double(elapsed));
	}
	// Sustaining
	else if (elapsed <= adsr.sTot) {
		startLevel = adsr.dL;
		endLevel = adsr.sL;
		segmentTimeLength = adsr.s;
		segmentCompleted = elapsed - adsr.dTot;
	}
	// Releasing
	else if (elapsed <= adsr.rTot) {
		startLevel = adsr.sL;
		endLevel = 0.f;
		segmentTimeLength = adsr.r;
		segmentCompleted = elapsed - adsr.sTot;
		if (spline)
			return splineRelease(double(elapsed));
	}
	// Note has been completed
	else {
		return 0.f;
	}

	return utils::math::lerp(startLevel, endLevel, float(segmentCompleted) / float(segmentTimeLength));
}
