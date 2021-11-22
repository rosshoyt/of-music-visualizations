#pragma once
#include "MidiAnimationGUIComponent.h"
#include "Utils.h"
#include <iostream>
#include <vector>

class ModalVisualizer : public MIDIAnimationGUIComponent {
public:
	ModalVisualizer(std::string uid = "ModalVisualizer");
	// Inherited via MIDIAnimationGUIComponent
	void setupGUI() override;
	void setup() override;
	void update() override;
	void draw() override;
	void resized(int w, int h) override;

private:
	ofEasyCam mainCam;

	std::vector<ofNode> nodes;
	ofPolyline line;
	ofNode baseNode;
	ofNode childNode;
	ofNode grandChildNode;


	void updateNodes();
};