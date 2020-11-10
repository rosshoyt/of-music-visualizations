#include "Utils.h"

std::pair<int, int> utils::getOctavePitchPair(int midiNoteNumber) {
	int octave = midiNoteNumber / 12;
	int pitch = midiNoteNumber % 12;
	return { octave, pitch };
}

utils::note_grid::PointNoteMap::PointNoteMap() : pointNoteMap(), width(), height(), widthGrid(), heightGrid() {}

void utils::note_grid::PointNoteMap::setup(const ofMesh& mesh, int width, int height, int widthGrid, int heightGrid) {
	this->width = width, this->height = height, this->widthGrid = widthGrid, this->heightGrid = heightGrid;
	init(mesh);
}

// point passed in must be from original mesh 
// throws out_of_range exception if point does not exist in PointNoteMap

int utils::note_grid::PointNoteMap::getNote(const ofPoint& point) {
	return pointNoteMap.at({ point.x, point.y });
}

void utils::note_grid::PointNoteMap::init(const ofMesh& mesh) {
	// find the min/max vertex positions of x and y planes
	for (const auto& vertex : mesh.getVertices()) {
		widthMax = std::max(vertex.x, widthMax);
		widthMin = std::min(vertex.x, widthMin);
		heightMax = std::max(vertex.y, heightMax);
		heightMin = std::min(vertex.y, heightMin);

		int note = getNoteFromPoint(vertex);
		// associate the x,y coords of each vertex with the MIDI note they represent
		pointNoteMap.insert({ { vertex.x, vertex.y }, note });
	}

	width = widthMax - widthMin;
	height = heightMax - heightMin;

	std::cout << "Calculated width/height = " << width << "/" << height << ", widthMin/Max = " << widthMin << "/" << widthMax << ", heightMin/Max = " << heightMin << "/" << heightMax << "\n";
	/* for (const auto& vertex : mesh.getVertices()) {


	}*/
}

int utils::note_grid::PointNoteMap::getNoteFromPoint(const ofVec3f& point) {
	//std::cout << "Finding note for point (" << point << ").... ";
	int xSegment = getSegmentNumber(point.x, width, widthGrid, widthMin, widthMax);
	int ySegment = getSegmentNumber(point.y, height, heightGrid, heightMin, heightMax);
	// convert segment #s to MIDI note number
	int noteNumber = ySegment * heightGrid + xSegment;
	//std::cout<< "Pitch = " << noteNumber << ", xSegment #: " << xSegment << ", ySegment #: " << ySegment << std::endl;

	return noteNumber;

}

int utils::note_grid::PointNoteMap::getSegmentNumber(const float& coord, const float& dimLength, const int& nSegments, const float& minVal, const float& maxVal) {
	float segmentSize = dimLength / float(nSegments);

	// first normalize the coordinate to start from 0
	float normalizedCoord = coord - minVal;

	int pitchGridIndex = 1;


	/*float accum = float(coord);
	while (accum <= maxVal) {
	float gridSegmentMaxVal = pitchGridIndex * segmentSize;
	if (accum < gridSegmentMaxVal) {
	break;
	}
	accum += segmentSize;
	++pitchGridIndex;
	}*/


	for (; pitchGridIndex <= nSegments; pitchGridIndex++) {
		float gridSegmentMaxVal = pitchGridIndex * segmentSize;
		if (normalizedCoord <= gridSegmentMaxVal) {
			break;
		}
	}


	return pitchGridIndex - 1;

}