#pragma once
#include "AnimationComponent.h"
#include "GUIComponent.h"
#include "UID.h"
#include "MIDIPortStateUser.h"


class MIDIAnimationComponent : public AnimationComponent, public GUIComponent, public MIDIPortStateUser {
public:
	MIDIAnimationComponent(std::string uid) : GUIComponent(uid) {}
};
