#include "ofMain.h"
#include "ofxGui.h"
#include "MIDIAnimationComponent.h"
#include "Utils.h"


/**
* Class that manages state of 3D mesh animation
*/
class Animated3DMesh : public MIDIAnimationComponent {
public:
    Animated3DMesh(std::string uid = "3D Mesh");
    void setup();
    void update();
    void draw();
    void drawGUI();
    void keyPressed(int key);

private:
    //--------------------------------------------------------------
    // Private fields
    //--------------------------------------------------------------
    // 3D graphics
    ofMesh mainMesh;
    ofEasyCam mainCam;

    ofxPanel gui;
    ofParameter<float> displacementSlider;


    
    // TODO refactor shared variables between this class and NoteGridAnimation to a parent class
    int widthNoteGrid, heightNoteGrid;
    bool b_messyMesh, b_perlinMesh, b_drawWireFrame;
    float perlinRange, perlinHeight;

    int width = 50, height = 50;
    float rangeMult = 4;
    float defaultDisplacement = float(width) / 2.f;
    
    // map that stores each x/y coord pair's associated MIDI pitch value
    utils::note_grid::PointNoteMap pointNoteMap;
    
    //--------------------------------------------------------------
    // Private methods
    //-------------------------------------------------------------- 
   
};