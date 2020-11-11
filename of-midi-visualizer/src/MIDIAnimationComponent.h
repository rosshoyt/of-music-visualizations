#pragma once
#include "AnimationComponent.h"
#include "GUIComponent.h"
#include "UID.h"
#include "MIDIPortStateUser.h"


class MIDIAnimationComponent : public UID, public AnimationComponent, public GUIComponent, public MIDIPortStateUser {
public:
	MIDIAnimationComponent(std::string uid) : UID(uid) {}
};
