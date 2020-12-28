#pragma once
#include "GUIAnimationComponent.h"
#include "MIDIPortStateUser.h"

class MIDIAnimationGUIComponent : public GUIAnimationComponent, public MIDIPortStateUser {
public:
	MIDIAnimationGUIComponent(std::string uid) : GUIAnimationComponent(uid) {}
};
