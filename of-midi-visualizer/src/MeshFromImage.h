#pragma once
#include "MIDIAnimationComponent.h"
#include "ofMain.h"
#include "Utils.h"


class MeshFromImage :
	public MIDIAnimationComponent
{
public:
    MeshFromImage(std::string uid = "3D Mesh", std::string imagePath = "textures/pnw.jpg");
	
    void setup();
    //--------------------------------------------------------------
    void update();

    //--------------------------------------------------------------
    void draw();

    void drawGUI();

	
private:
	std::string imagePath;

    ofEasyCam easyCam;
    ofImage image;
    ofMesh mesh;
    std::vector<float> zVals;

    int width = 200, height = 200;

    utils::note_grid::PointNoteMap pointNoteMap;

};

