#pragma once
#include <string>
#include "ofMain.h"
#include "ofxGui.h"
#include "AppConstants.h"

extern const int ANIMATION_WIDTH, ANIMATION_HEIGHT;

class AnimationComponent {
public:
    AnimationComponent() = default;
    
	// method which should be called to initialize the AnimationComponent and prepare it to display at some point in the future.
	// memory allocations, expensive 1-time  initializations and file operations are examples that should be implemented in setup()
	virtual void setup() = 0;

	// method which should be called first when the AnimationComponent is changed-to. 
	// for example, when it is selected to be displayed from the dropdown menu
	// sets up any destructive rendering settings which may affect other animations
	virtual void prepare();

	// method which should be called first when an animation is being changed-from
	// for exmaple, when it is the current animation and another is selected from the dropdown menu
	virtual void takedown();

	virtual void update() = 0;

	virtual void draw() = 0;
    
    virtual void resized(int w, int h);
    
    static void setAnimationDimensions(float width, float height);
    
    static void setAnimationWidth(float w);
    
    static void setAnimationHeight(float h);
    
    static float getAnimationWidth();
    
    static float getAnimationHeight();

    static ofVec2f getAnimationCenterPosition();

private:
	static float animationWidth, animationHeight; // to track the sub-window space that the animation component displays on
    
};
