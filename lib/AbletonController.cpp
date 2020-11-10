#include "AbletonController.h"

AbletonController::AbletonController() : isPlaying(false) {
	live.setup();
}

void AbletonController::processSpacebar() {
	//std::cout << "playing\n";
	if (!isPlaying) {
		play();
		isPlaying = true;
	}
	else {
		stop();
		isPlaying = false;
	}
}

ofParameter<float> AbletonController::getTempo() {
	return live.getTempo();
}

void AbletonController::play() {
	live.play();
}

void AbletonController::stop() {
	live.stop();
}
