#pragma once
#include "AnimationComponent.h"
#include "ofApp.h"

class MeshFromImage :
	public AnimationComponent
{
public:
	MeshFromImage(std::string uid, std::string imagePath = "textures/hubble.png") : AnimationComponent(uid), imagePath(imagePath) {

	}
	
	void setup() {
        image.load(imagePath);
        image.resize(200, 200);

        // Don't forget to change to lines mode!
        mesh.setMode(OF_PRIMITIVE_LINES);

        // We are going to be using indices this time
        mesh.enableIndices();

        mesh.enableColors();

        float intensityThreshold = 150.0;
        int w = image.getWidth();
        int h = image.getHeight();
        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < h; ++y) {
                ofColor c = image.getColor(x, y);
                float intensity = c.getLightness();

                if (intensity >= intensityThreshold) {
                    float saturation = c.getSaturation();
                    float z = ofMap(saturation, 0, 255, -100, 100);
                    ofVec3f pos(x * 4, y * 4, z);
                    mesh.addVertex(pos);
                    mesh.addColor(c);
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
	}
    //--------------------------------------------------------------
    void update() {

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

};

