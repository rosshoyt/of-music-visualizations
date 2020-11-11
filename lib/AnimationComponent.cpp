#include "AnimationComponent.h"

//AnimationComponent::AnimationComponent() {
//
//}

void AnimationComponent::setAnimationDimensions(float width, float height) {
	animationWidth = width, animationHeight = height;
}

// method which should be called first when the AnimationComponent is changed-to. 
// for example, when it is selected to be displayed from the dropdown menu
// sets up any destructive rendering settings which may affect other animations

void AnimationComponent::prepare() {}

// method which should be called first when an animation is being changed-from
// for exmaple, when it is the current animation and another is selected from the dropdown menu

void AnimationComponent::takedown() {}

//void AnimationComponent::drawBackground() {}

void AnimationComponent::resized(int w, int h) {}




