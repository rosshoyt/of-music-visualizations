#include "Animated3DMesh.h"

Animated3DMesh::Animated3DMesh(std::string uid) : MIDIAnimationComponent(uid), pointNoteMap() {}

//--------------------------------------------------------------
void Animated3DMesh::setup() {
    
    
    gui.setup();
    gui.setPosition(menuX, menuY);
    //std::cout << "menuX/y" << menuX << " " << menuY;
    gui.add(displacementSlider.set("Displacement", defaultDisplacement,-defaultDisplacement * rangeMult, defaultDisplacement  * rangeMult));


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

    //mainMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    // make points inside the mesh
    // add one vertex to the mesh across our width and height
    // we use these y and x vals to set the x and y coordinates of the mesh, adding a z value of zero to complete the 3D location of each vertex
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ofPoint point(x - width / 2, y - height / 2, 0);
            mainMesh.addVertex(point);

            
            // Find the color for the row/column
            // TODO create shared function set for these utils
            //int row = heightNoteGrid - 1 - note / 12, col = note % widthNoteGrid;
            //ofFloatColor().lerp

            ofFloatColor start = ofFloatColor::darkBlue, end = ofFloatColor::lightSeaGreen;
            mainMesh.addColor(start.lerp(end, std::max(float(y) / float(height), float(x)/float(width))));
            
            
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
    // setup the pointNoteMap
    pointNoteMap.setup(mainMesh, width, height, widthNoteGrid, heightNoteGrid);

}

//--------------------------------------------------------------
void Animated3DMesh::update() {
    auto allNotesDown = midiPortState->getAllNotesDown();

    // Update position of vertices based on if there is a note at their location
    // TODO could optimize by reversing pointNoteMap to notePointMap 
    int numVertices = mainMesh.getNumVertices();
    for (int i = 0; i < numVertices; i++) {
        ofVec3f newPosition = mainMesh.getVertex(i);
        int midiPitch = pointNoteMap.getNote(newPosition);
            if (allNotesDown.count(midiPitch) > 0) {
                // TODO scale based on SIN of frequency of pitch
                // like: float sinMod   = sin(timePassed, frequency);
                
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

    // TODO refactor - do in shader?
    float offset = displacementSlider;//float(width) / 2.f;
    
    float depthDisplacement = defaultDisplacement - displacementSlider;
    // front
    ofPushMatrix();
    ofTranslate(0, 0, depthDisplacement);
    mainMesh.drawWireframe();
    ofPopMatrix();

    // right
    ofPushMatrix();
    ofTranslate(offset, 0.f, -offset);
    ofRotateY(90.f);
    mainMesh.drawWireframe();
    ofPopMatrix();
    
    // back
    ofPushMatrix();
    ofTranslate(0.f, 0.f, depthDisplacement * 2 - width);/*-width //TODO create depth var? */
    ofRotateY(180.f);
    mainMesh.drawWireframe();
    ofPopMatrix();

    // left
    ofPushMatrix();
    ofTranslate(-offset, 0.f, -offset);
    ofRotateY(270.f);
    mainMesh.drawWireframe();
    ofPopMatrix();

    // top
    ofPushMatrix();
    ofTranslate(0.f, offset, -offset);
    ofRotateX(270.f);
    mainMesh.drawWireframe();
    ofPopMatrix();

    // bottom
    ofPushMatrix(); 
    ofTranslate(0.f, -offset, -offset);
    ofRotateX(90.f);
    mainMesh.drawWireframe();
    ofPopMatrix();


    mainCam.end();

    gui.draw();
}

void Animated3DMesh::drawGUI() {
    
}

//--------------------------------------------------------------
void Animated3DMesh::keyPressed(int key) {
    switch (key) {
    case 'f':
        ofToggleFullscreen();
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