#include "ofMain.h"
#include "ofxGui.h"
#include "MIDIAnimationGUIComponent.h"
#include "Utils.h"



/**
* Class that manages state of 3D mesh animation
*/
class Animated3DMesh : public MIDIAnimationGUIComponent {
public:
    Animated3DMesh(std::string uid = "3D Mesh");
    void setup();
    void setupGUI() override;
    void update();
    void draw();
    void keyPressed(int key);
    void resized(int w, int h) override;

private:
    //--------------------------------------------------------------
    // Private fields
    //--------------------------------------------------------------
    // 3D graphics
    ofMesh mainMesh;
    ofEasyCam mainCam;

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
};