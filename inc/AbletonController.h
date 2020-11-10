#pragma once
#include "ofxAbletonLive.h"
/**
* Class which wraps ofxAbleton for simple playback control
*/
class AbletonController
{
public:
	AbletonController();

	void processSpacebar();

	ofParameter<float> getTempo();

private:
	ofxAbletonLive live;

	std::atomic<bool> isPlaying;

	void play();
	void stop();
	

};

