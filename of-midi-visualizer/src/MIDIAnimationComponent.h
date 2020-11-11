#pragma once
#include "AnimationComponent.h"
#include "MIDIPortStateUser.h"
#include "GUIComponent.h"

class MIDIAnimationComponent : public AnimationComponent, public GUIComponent, public MIDIPortStateUser {
public:
	MIDIAnimationComponent(std::string uid) : AnimationComponent(uid) {}
};
