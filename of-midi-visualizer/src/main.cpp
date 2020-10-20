#include "ofMain.h"
#include "ofApp.h"

extern const int HEIGHT = 1024, WIDTH = 1024, RIGHT_CONTROLBAR = 300;

//========================================================================
int main( ){
	ofSetupOpenGL(WIDTH + RIGHT_CONTROLBAR, HEIGHT,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
