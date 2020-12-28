#include "MeshFromImage.h"

//--------------------------------------------------------------
MeshFromImage::MeshFromImage(std::string uid, std::string imagePath) : MIDIAnimationGUIComponent(uid), imagePath(imagePath), pointNoteMap(), zVals() {
	// "textures/treeCrop.png"
	// textures/allRGBV.png
	// textures/nebulabrotCrop.png
}

//--------------------------------------------------------------
void MeshFromImage::setup() {
	image.load(imagePath);
	image.resize(width, height);

	// Change to lines mode!
	mesh.setMode(OF_PRIMITIVE_LINES);

	// We are using indices
	mesh.enableIndices();

	mesh.enableColors();

	float intensityThreshold = 130.f;
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
	pointNoteMap.setup(mesh, width, height, 12, 12);
}

//--------------------------------------------------------------
void MeshFromImage::setupGUI() {
	gui.add(positionScale.set("Note Scale", _defPositionScale, -4000, 4000));
}

//--------------------------------------------------------------
void MeshFromImage::update() {
	auto allNotesDown = midiPortState->getAllNotesDown();
	for (int i = 0; i < mesh.getNumVertices(); ++i) {
		auto position = mesh.getVertex(i);
		int midiPitch = pointNoteMap.getNote(position);

		if (allNotesDown.count(midiPitch) > 0) {
			auto velocityADSR = allNotesDown[midiPitch];
			position.z = zVals[i] + positionScale * velocityADSR.first / 128.f * velocityADSR.second;
		}
		else {
			position.z = zVals[i];
		}
		mesh.setVertex(i, position);
	}
}

//--------------------------------------------------------------
void MeshFromImage::draw() {
	easyCam.begin();
	ofPushMatrix();
	ofTranslate(-ofGetWidth() / 3, -ofGetHeight() / 3);
	mesh.draw();
	ofPopMatrix();
	easyCam.end();
}

