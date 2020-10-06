#include "ofMain.h"
#include "MIDIPortState.h"
#include "AnimationComponent.h"

/**
* Class that manages state of 3D mesh animation
*/
class Animated3DMesh : public AnimationComponent {
public:
    Animated3DMesh(std::string uid, MIDIPortState* midiPortState);
    void setup();
    void update();
    void draw();
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
    int widthNoteGrid, heightNoteGrid;
    bool b_messyMesh, b_perlinMesh, b_drawWireFrame;
    float perlinRange, perlinHeight;
    // map that stores each x/y coord pair's associated MIDI pitch value
    std::map<std::pair<int, int>, int> pointNoteMap;
    
    //--------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------
    
    int getNoteFromPoint(const ofVec3f& point);
    // TODO refactor, store some calculated vars globally
    int getSegmentNumber(const int coord, const int dimLength, const int nSegments);
    
};