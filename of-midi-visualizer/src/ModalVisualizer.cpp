#include "ModalVisualizer.h"

ModalVisualizer::ModalVisualizer(std::string uid) : MIDIAnimationGUIComponent(uid)
{
}

void ModalVisualizer::setupGUI()
{
}

void ModalVisualizer::setup()
{
}

void ModalVisualizer::update()
{
}

void ModalVisualizer::draw()
{	
	// TODO create method AnimationComponent::getSmallestDimension()
	float smallestDim = AnimationComponent::getAnimationWidth() > AnimationComponent::getAnimationHeight() ? AnimationComponent::getAnimationHeight() : AnimationComponent::getAnimationWidth();
	
	float radius = smallestDim / 6; // radius of spiral array
	float height = 50; // height of quarter rotation 
	
	// promote center point to 3D point
	ofVec3f centerPoint(AnimationComponent::getAnimationCenterPosition());

	// initialize first point to the center of the window
	ofVec3f initialPoint(centerPoint);
	std::cout << "Drawing spiral array\n";
	for (int i = 1; i < 12; i++) {
		int k = (i * 7) % 12; // the spiral array pitch index (5th interval distance from root)

		// calculate next point on the spiral array
		ofVec3f newPoint = ofVec3f(radius * sinf(k * utils::math::pi / 2), radius * cosf(k * utils::math::pi / 2), k * height) + centerPoint;

		std::cout << "Drawing line between " << initialPoint << " and " << newPoint << '\n';

		ofDrawLine(initialPoint,  newPoint);
		initialPoint = newPoint;
	}
}
