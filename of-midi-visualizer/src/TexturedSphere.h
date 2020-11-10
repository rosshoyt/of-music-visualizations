#pragma once
#include "ofMain.h"
#include "MIDIAnimationComponent.h"

class TexturedSphere : public MIDIAnimationComponent {
public:
	TexturedSphere(std::string uid = "Textured Sphere");

	void setup();

	void prepare() override;

	void takedown() override;

	void update();

	void draw();

	void drawGUI();

	
private:
	ofSpherePrimitive sphere;
	ofLight light;
	ofTexture mTex;
	ofEasyCam cam;

};