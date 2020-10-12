#include "ofMain.h"
#include "MIDIPortState.h"
#include "AnimationComponent.h"
#include "Utils.h"

/**
* Class that manages state of 3D mesh animation
*/
class Animated3DMesh : public AnimationComponent {
public:
    Animated3DMesh(std::string uid, MIDIPortState* midiPortState);
    void setup();
    void update();
    void draw();
    void drawGUI() override;
    void keyPressed(int key);

private:
    //--------------------------------------------------------------
    // Private fields
    //--------------------------------------------------------------
    MIDIPortState* midiPortState;
    // 3D graphics
    ofMesh mainMesh;
    ofEasyCam mainCam;

    int width, height;
    // TODO refactor shared variables between this class and NoteGridAnimation to a parent class
    int widthNoteGrid, heightNoteGrid;
    bool b_messyMesh, b_perlinMesh, b_drawWireFrame;
    float perlinRange, perlinHeight;
    
    // map that stores each x/y coord pair's associated MIDI pitch value
    utils::note_grid::PointNoteMap pointNoteMap;
    
    //--------------------------------------------------------------
    // Private methods
    //-------------------------------------------------------------- 
   
};