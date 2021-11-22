#include "ModalVisualizer.h"

ModalVisualizer::ModalVisualizer(std::string uid) : MIDIAnimationGUIComponent(uid), nodes()
{
}

void ModalVisualizer::setupGUI()
{
}

void ModalVisualizer::setup()
{
	baseNode.setPosition(0, 0, 0);
	childNode.setParent(baseNode);
	childNode.setPosition(0, 0, 200);
	grandChildNode.setParent(childNode);
	grandChildNode.setPosition(0, 50, 0);

}

void ModalVisualizer::update()
{
	baseNode.pan(1);
	childNode.tilt(3);

	line.addVertex(grandChildNode.getGlobalPosition());
	if (line.size() > 200) {
		line.getVertices().erase(
			line.getVertices().begin()
		);
	}
}

void ModalVisualizer::draw()
{	
	mainCam.begin();
	
	line.draw();

	mainCam.end();
}

// ala https://openframeworks.cc/ofBook/chapters/lines.html
void ModalVisualizer::updateNodes() 
{
	//std::vector<ofNode> newNodes;
	//// TODO create method AnimationComponent::getSmallestDimension()
	//float smallestDim = AnimationComponent::getAnimationWidth() > AnimationComponent::getAnimationHeight() ? AnimationComponent::getAnimationHeight() : AnimationComponent::getAnimationWidth();

	//float radius = smallestDim / 6; // radius of spiral array
	//float height = 50; // height of quarter rotation 

	//ofVec3f centerPoint(AnimationComponent::getAnimationCenterPosition());

	//// initialize first point to the center of the window
	////ofVec3f initialPoint(centerPoint);

	//ofNode baseNode;
	//baseNode.setPosition(centerPoint);

	//std::cout << "Updating  nodes array\n";
	//for (int i = 1; i < 12; i++) {
	//	int k = (i * 7) % 12; // the spiral array pitch index (5th interval distance from root)

	//	// calculate next point on the spiral array
	//	ofVec3f newPoint((radius * sinf(k * utils::math::pi / 2), radius * cosf(k * utils::math::pi / 2), k * height) + centerPoint);

	//	std::cout << "New point at " << newPoint << ", setting  node's position to this.\n";

	//	ofNode newNode;
	//	newNode.setPosition(newPoint);
	//	newNode.setParent(baseNode);
	//	

	//	//ofDrawLine(initialPoint, newPoint);
	//	baseNode = newNode;

	//	newNodes.push_back(newNode);
	//}

	//std::cout << "Nodes list:[";
	//for (auto& node : newNodes) {

	//	std::cout << "(" <<node.getPosition() << "), ";
	//}
	//std::cout << "]\n";
	//nodes = newNodes;
	//line.addVertex(baseNode.getGlobalPosition());

}

//--------------------------------------------------------------
void ModalVisualizer::resized(int w, int h) {
	// displace the camera to the right by half of the size of the menu bar
	// TODO allow each app to access the size of the GUI menu bar on the side
	// TODO along with other variables via a DTO struct
	float xDisplacement = (w - AnimationComponent::getAnimationWidth()) / 25.f; // TODO figure out better way to move mesh to left
	mainCam.setPosition(xDisplacement, 0, 80);
	std::cout << "xDisplacement: " << xDisplacement << "\n";

	updateNodes();

}