#pragma once
#include "AnimationComponent.h"
#include "MIDIPortStateUser.h"

class MIDIAnimationComponent : public AnimationComponent, public MIDIPortStateUser {
public:
	MIDIAnimationComponent(std::string uid) : AnimationComponent(uid) {}
};
