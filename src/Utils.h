#pragma once
#include "ofMain.h"

namespace utils {
    
    namespace note_grid {

        

        class PointNoteMap {
        public:
            PointNoteMap() : pointNoteMap(), width(), height(), widthGrid(), heightGrid() {
            
            }
            
            void setup(const ofMesh& mesh, int width, int height, int widthGrid, int heightGrid) {
                this->width = width, this->height = height, this->widthGrid = widthGrid, this->heightGrid = heightGrid;
                init(mesh);
            }
            // point passed in must be from original mesh 
            // throws out_of_range exception if point does not exist in PointNoteMap
            int getNote(const ofPoint& point) {
                return pointNoteMap.at({ point.x, point.y });
            }
            
        private:
            int width, height, widthGrid, heightGrid;
            std::map<std::pair<int, int>, int> pointNoteMap;


            void init(const ofMesh& mesh) {
                for (const auto& vertex : mesh.getVertices()) {
                    int note = getNoteFromPoint(vertex, width, height, widthGrid, heightGrid);
                    // associate the x,y coords of each vertex with the MIDI note they represent
                    pointNoteMap.insert({ { vertex.x, vertex.y }, note });

                }
            }


            int getNoteFromPoint(const ofVec3f& point, const int& width, const int& height, const int& gridHeight, const int& gridWidth) {
                //std::cout << "Finding note for point (" << point << ")" << std::endl;
                int xSegment = getSegmentNumber(point.x, width, gridWidth);
                int ySegment = getSegmentNumber(point.y, height, gridHeight);
                // convert segment #s to MIDI note number
                int noteNumber = ySegment * gridHeight + xSegment;
                //std::cout<< "Pitch = " << noteNumber << ", xSegment #: " << xSegment << ", ySegment #: " << ySegment << std::endl;

                return noteNumber;

            }
            int getSegmentNumber(const int coord, const int dimLength, const int nSegments) {
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
        };

       
    }
}
