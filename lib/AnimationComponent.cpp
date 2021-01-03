#include "AnimationComponent.h"

float AnimationComponent::animationWidth = ANIMATION_WIDTH;

float AnimationComponent::animationHeight = ANIMATION_HEIGHT;

void AnimationComponent::setAnimationDimensions(float width, float height) {
	animationWidth = width, animationHeight = height;
}


void AnimationComponent::setAnimationWidth(float w){
    animationWidth = w;
}
void AnimationComponent::setAnimationHeight(float h){
    animationHeight = h;
}

float AnimationComponent::getAnimationWidth(){
    return animationWidth;
}

float AnimationComponent::getAnimationHeight(){
    return animationHeight;
}

void AnimationComponent::prepare() {}

void AnimationComponent::takedown() {}

void AnimationComponent::resized(int w, int h) {}
