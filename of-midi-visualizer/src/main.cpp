#include "ofMain.h"
#include "ofApp.h"
#include "AppConstants.h"

extern const int ANIMATION_WIDTH, ANIMATION_HEIGHT, OFXGUI_DEF_WIDTH, RIGHT_CONTROLBAR;
//========================================================================
int main( ){
	ofSetupOpenGL(ANIMATION_WIDTH + RIGHT_CONTROLBAR, ANIMATION_HEIGHT, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
