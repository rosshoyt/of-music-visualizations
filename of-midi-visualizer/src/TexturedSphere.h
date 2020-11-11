#pragma once
#include "ofMain.h"
#include "MIDIAnimationComponent.h"

class TexturedSphere : public MIDIAnimationComponent {
public:
	TexturedSphere(std::string uid = "Textured Sphere");
	void setup();
	void setupGUI() override;
	void prepare() override;
	void takedown() override;
	void update();
	void draw();

private:
	ofSpherePrimitive sphere;
	ofLight light;
	ofTexture mTex;
	ofEasyCam cam;

};