#pragma once
#include "MIDIAnimationGUIComponent.h"
#include "ofMain.h"
#include "Utils.h"


class MeshFromImage :
	public MIDIAnimationGUIComponent
{
public:
    // TODO add multiple mesh support (search all pictures in a directory?) support async loading (may holdup build times)
    MeshFromImage(std::string uid = "Mesh From Image", std::string imagePath = "textures/allRGBV.png"); // 21lawrence-superJumbo.jpg
    void setup();
    void setupGUI() override;
    void update();
    void draw();

private:
	std::string imagePath;

    ofEasyCam easyCam;
    ofImage image;
    ofMesh mesh;
    std::vector<float> zVals;

    int width = 200, height = 200;

    utils::note_grid::PointNoteMap pointNoteMap;

    // Gui Parameters
    ofParameter<float> positionScale;
    const float _defPositionScale = 350;
};

