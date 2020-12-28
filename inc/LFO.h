#pragma once
#include <math.h>
#include "Utils.h"

class LFO{
public:
	
	LFO(double timeLengthMS = 100.f) : timeLengthMS(timeLengthMS) {}

	virtual double getValue(double timeSinceStartMS) {
		return 0;
	}

	void setTimeLength(double timeLengthMS) {
		this->timeLengthMS = timeLengthMS;
	}

protected:
	double timeLengthMS;

};

class SineLFO : public LFO {
public:
	double getValue(double timeSinceStart) override {
		auto percentComplete = fmod(timeSinceStart, timeLengthMS) / timeLengthMS;
		auto value = std::sin(percentComplete * utils::math::pi);
		// scale value to be between 0 and 1
		return (value + 1.f) / 2.f;
	}

private:

};