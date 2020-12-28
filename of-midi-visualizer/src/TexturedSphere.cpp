#include "TexturedSphere.h"

//--------------------------------------------------------------
TexturedSphere::TexturedSphere(std::string uid) : MIDIAnimationGUIComponent(uid) {}

//--------------------------------------------------------------
void TexturedSphere::setup() {
	ofDisableAlphaBlending();
	ofEnableDepthTest();
	light.enable();
	light.setPosition(ofVec3f(100, 100, 200));
	light.lookAt(ofVec3f(0, 0, 0));
	ofLoadImage(mTex, "textures/earth.jpg");
	mTex.generateMipmap();
	mTex.setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	ofDisableArbTex();
}

//--------------------------------------------------------------
void TexturedSphere::setupGUI() {}

//--------------------------------------------------------------
void TexturedSphere::prepare() {}

//--------------------------------------------------------------
void TexturedSphere::takedown() {
	light.disable();
	ofEnableAlphaBlending();
	ofDisableDepthTest();
	ofEnableArbTex();
}

//--------------------------------------------------------------
void TexturedSphere::update() {}

//--------------------------------------------------------------
void TexturedSphere::draw() {
	cam.begin();
	mTex.bind();
	sphere.draw();
	mTex.unbind();
	cam.end();
}
