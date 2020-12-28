#pragma once
#include "AnimationComponent.h"
#include "GUIComponent.h"

class GUIAnimationComponent : public GUIComponent, public AnimationComponent {
public:
	GUIAnimationComponent(std::string uid) : GUIComponent(uid) {}

};

