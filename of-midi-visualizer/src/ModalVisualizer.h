#pragma once
#include "MidiAnimationGUIComponent.h"
#include "Utils.h"
#include <iostream>

class ModalVisualizer : public MIDIAnimationGUIComponent {
public:
	ModalVisualizer(std::string uid = "ModalVisualizer");
	// Inherited via MIDIAnimationGUIComponent
	void setupGUI() override;
	void setup() override;
	void update() override;
	void draw() override;
};