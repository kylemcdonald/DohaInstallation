#include "PhotoWall.h"

PhotoWall::PhotoWall() {
}

void PhotoWall::setup(const ControlSurface& surface) {
	const ofxVec2f& divisions = surface.getDivisions();
	for(int i = 0; i < divisions.y - 1; i++) {
		for(int j = 0; j < divisions.x - 1; j++) {
			photos.push_back(Photo());
			photos.back().setup(surface, j + .5, i + .5);
		}
	}
}

void PhotoWall::update() {
	for(unsigned int i = 0; i < photos.size(); i++)
		photos[i].update();
}

void PhotoWall::draw() const {
	for(unsigned int i = 0; i < photos.size(); i++)
		photos[i].draw();
}