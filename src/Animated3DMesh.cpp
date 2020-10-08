#include "Animated3DMesh.h"

Animated3DMesh::Animated3DMesh(std::string uid, MIDIPortState* midiPortState) : AnimationComponent(uid), pointNoteMap(), midiPortState(midiPortState) {}

//--------------------------------------------------------------
void Animated3DMesh::setup() {
    width = 50;
    height = 50;

    widthNoteGrid = 12;
    heightNoteGrid = 12;

    // set rendering styles to false;
    b_messyMesh = false;
    b_perlinMesh = false;
    b_drawWireFrame = true;

    // set initial values to use for perlin noise
    perlinRange = 1.0;
    perlinHeight = 5.0;


    mainCam.setPosition(0, 0, 80);

    // make points inide the mesth
    // add one vertex to the mesh across our width and height
    // we use these y and x vals to set the x and y coordinates of the mesh, adding a z value of zero to complete the 3D location of each vertex
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ofPoint point(x - width / 2, y - height / 2, 0);
            mainMesh.addVertex(point);
            mainMesh.addColor(ofFloatColor(0, 0, 0));
            // associate the x,y coords of each vertex with the MIDI note they represent
            pointNoteMap.insert({ { point.x, point.y }, getNoteFromPoint(point) });
        }
    }

    // here we loop through and join the vertices together as indices to make rows of triangles to make the wireframe grid
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {

            int index1 = x + y * width;
            int index2 = (x + 1) + y * width;
            int index3 = x + (y + 1) * width;

            int index4 = (x + 1) + y * width;
            int index5 = (x + 1) + (y + 1) * width;
            int index6 = x + (y + 1) * width;

            //printf("%d %d %d %d %d %d\n",index1, index2, index3, index4, index5, index6);

            mainMesh.addIndex(index1);
            mainMesh.addIndex(index2);
            mainMesh.addIndex(index3);

            mainMesh.addIndex(index4);
            mainMesh.addIndex(index5);
            mainMesh.addIndex(index6);
        }
    }
}

//--------------------------------------------------------------
void Animated3DMesh::update() {
    // Get the current active MIDI notes for all channels and
    // store in map<midiPitch, <velocity, adsr value>>
    std::map<int, std::pair<int,float>> allNotesDown;
    int channelNum = 0;
    for (auto channelNotes : midiPortState->getAllChannelNotes()) {
        for (auto note : channelNotes) {
            // TODO don't overwrite values when 2 notes are same between channels
            allNotesDown.insert({ note.first, { note.second, midiPortState->getADSRValue(channelNum, note.first) } });
        }
        ++channelNum;
    }


    // Update position of vertices based on if there is a note at their location
    // TODO could optimize by reversing pointNoteMap to notePointMap 
    int numVertices = mainMesh.getNumVertices();
    for (int i = 0; i < numVertices; i++) {
        ofVec3f newPosition = mainMesh.getVertex(i);
        int midiPitch = pointNoteMap.at({ newPosition.x, newPosition.y });
            if (allNotesDown.count(midiPitch) > 0) {
                // TODO scale based on SIN of frequency of pitch
                // like: float sinMod = sin(timePassed, frequency);
                
                // scale note on velocity along with adsr value (TODO redundant scaling? ADSR takes velocity into account)
                auto velocityADSR = allNotesDown[midiPitch];
                newPosition.z = 20.f * velocityADSR.first / 128.f * velocityADSR.second; // * sinMod; TODO
            }
            else {
                // no note present - set z to 0
                newPosition.z = 0.0f;
            }
        
        mainMesh.setVertex(i, newPosition);
    }
}

//--------------------------------------------------------------
void Animated3DMesh::draw() {
    mainCam.begin();

    if (b_drawWireFrame) {
        mainMesh.drawWireframe();
    }
    else {
        mainMesh.drawVertices();
    }
    mainCam.end();

    ofSetColor(100);
    string msg = "f: toggle full screen, spacebar: random z-value in meshvertices, w: draw wireframe or point cloud \np: use PerlinNoise for z-value in meshvertices\nUp-key Down-key: increase/devrease PerlinNoise input range\nRight-key Left-key: increase/decrease amplitude of Perlin Noise distortion\nclick and drag in window to move camera";
    ofDrawBitmapString(msg, 10, 20);
}

void Animated3DMesh::drawGUI() {

}

//--------------------------------------------------------------
void Animated3DMesh::keyPressed(int key) {
    switch (key) {
    case 'f':
        ofToggleFullscreen();
        break;
    case ' ':
        b_messyMesh = !b_messyMesh;
        break;
    case 'w':
        b_drawWireFrame = !b_drawWireFrame;
        break;
    case 'p':
        b_perlinMesh = !b_perlinMesh;
        break;

    case OF_KEY_UP:
        perlinRange += 0.1;
        break;
    case OF_KEY_DOWN:
        if (perlinRange > 0.1)
            perlinRange -= 0.1;
        break;

    case OF_KEY_RIGHT:
        perlinHeight += 0.1;
        break;
    case OF_KEY_LEFT:
        if (perlinHeight > 0.1)
            perlinHeight -= 0.1;
        break;
    }
}

//--------------------------------------------------------------
int Animated3DMesh::getNoteFromPoint(const ofVec3f& point) {
    std::cout << "Finding note for point (" << point << ")" << std::endl;
    int xSegment = getSegmentNumber(point.x, width, widthNoteGrid);
    int ySegment = getSegmentNumber(point.y, height, heightNoteGrid);
    // convert segment #s to MIDI note number
    int noteNumber = ySegment * heightNoteGrid + xSegment;
    //std::cout<< "Pitch = " << noteNumber << ", xSegment #: " << xSegment << ", ySegment #: " << ySegment << std::endl;

    return noteNumber;

}

//--------------------------------------------------------------
// TODO refactor, store some calculated vars globally
int Animated3DMesh::getSegmentNumber(const int coord, const int dimLength, const int nSegments) {
    float segmentSize = float(dimLength) / float(nSegments);
    // first compensate for the offset which was introduced to center the mesh
    float coordOffset = dimLength / 2;
    int normalizedCoord = coord + coordOffset;

    int pitchGridIndex = 1;
    for (; pitchGridIndex <= nSegments; pitchGridIndex++) {
        float gridSegmentMaxVal = pitchGridIndex * segmentSize;
        if (normalizedCoord < gridSegmentMaxVal) {
            break;
        }
    }
    return pitchGridIndex - 1;

}