#pragma once
#include "GUIAnimationComponent.h"
#include "MIDIPortStateUser.h"


class MIDIAnimationComponent : public GUIAnimationComponent, public MIDIPortStateUser {
public:
	MIDIAnimationComponent(std::string uid) : GUIAnimationComponent(uid) {}
};
