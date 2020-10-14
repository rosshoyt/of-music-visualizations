#pragma once
#include "AnimationComponent.h"
#include "ofApp.h"
#include "Utils.h"
class MeshFromImage :
	public AnimationComponent
{
public:
	MeshFromImage(MIDIPortState* midiPortState, std::string uid, std::string imagePath = "textures/treeCrop.png") : AnimationComponent(midiPortState, uid), imagePath(imagePath), pointNoteMap(), zVals() {
        // textures/allRGBV.png
        // textures/nebulabrotCrop.png
	}
	
	void setup() {      
        image.load(imagePath);
        image.resize(width, height);

        // Don't forget to change to lines mode!
        mesh.setMode(OF_PRIMITIVE_LINES);

        // We are going to be using indices this time
        mesh.enableIndices();

        mesh.enableColors();

        float intensityThreshold = 75.f;
        int w = image.getWidth();
        int h = image.getHeight();
        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < h; ++y) {
                ofColor c = image.getColor(x, y);
                float intensity = c.getLightness();

                if (intensity >= intensityThreshold) {
                    float saturation = c.getSaturation();
                    float z = ofMap(saturation, 0, 255, -100, 100); // TODO fix scaling
                    ofVec3f pos(x * 4, y * 4, z);
                    mesh.addVertex(pos);
                    mesh.addColor(c);
                    // track original z values for use in animation
                    zVals.push_back(z);
                }
            }
        }


        // Let's add some lines!
        float connectionDistance = 30;
        int numVerts = mesh.getNumVertices();
        for (int a = 0; a < numVerts; ++a) {
            ofVec3f verta = mesh.getVertex(a);
            for (int b = a + 1; b < numVerts; ++b) {
                ofVec3f vertb = mesh.getVertex(b);
                float distance = verta.distance(vertb);
                if (distance <= connectionDistance) {
                    // In OF_PRIMITIVE_LINES, every pair of vertices or indices will be
                    // connected to form a line
                    mesh.addIndex(a);
                    mesh.addIndex(b);
                }
            }
        }

        std::cout << "setting up MeshFromImage pointNoteMap\n";
        pointNoteMap.setup(mesh, width, height, 12, 12);
	}
    //--------------------------------------------------------------
    void update() {
        auto allNotesDown = getAllNotesDown();
        //std::cout << allNotesDown.size() << " notes down\n";
        for (int i = 0; i < mesh.getNumVertices(); ++i) {
            auto position = mesh.getVertex(i);
            int midiPitch = pointNoteMap.getNote(position);

            if (allNotesDown.count(midiPitch) > 0) {
                auto velocityADSR = allNotesDown[midiPitch];
                position.z = zVals[i]  + 300.f * velocityADSR.first / 128.f * velocityADSR.second;
            }
            else {
                position.z = zVals[i];
            }

            mesh.setVertex(i, position);
        }
    }

    //--------------------------------------------------------------
    void draw() {
        ofColor centerColor = ofColor(85, 78, 68);
        ofColor edgeColor(0, 0, 0);
        ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);


        


        easyCam.begin();
        ofPushMatrix();
        ofTranslate(-ofGetWidth() / 3, -ofGetHeight() / 3);
        mesh.draw();
        ofPopMatrix();
        easyCam.end();
    }

	void drawGUI() {

	}

	
private:
	std::string imagePath;

    ofEasyCam easyCam;
    ofImage image;
    ofMesh mesh;
    std::vector<float> zVals;

    int width = 200, height = 200;

    utils::note_grid::PointNoteMap pointNoteMap;

};

