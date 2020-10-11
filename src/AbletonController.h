#pragma once
#include "ofxAbletonLive.h"
/**
* Class which wraps ofxAbleton for simple playback control
*/
class AbletonController
{
public:
	AbletonController() : isPlaying(false) {
		live.setup();
	}

	void processSpacebar() {
		if (!isPlaying) {
			play();
			isPlaying = true;
		}
		else {
			stop();
			isPlaying = false;
		}
	}

	ofParameter<float> getTempo() {
		return live.getTempo();
	}

private:
	ofxAbletonLive live;

	std::atomic<bool> isPlaying;

	void play() {
		live.play();
	}
	void stop() {
		live.stop();
	}
	

};

