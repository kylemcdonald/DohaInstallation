#pragma once

/*
 ofxVectorMath needs ofxVec2i etc.
 */

#include "ofxVectorMath.h"
#include "ControlPoint.h"
#include "ofMain.h"

class ControlSurface {
protected:
	ofxVec2f size, divisions;
	vector<ControlPoint> grid;
public:
	ControlSurface() {
	}
	void setup(const ofxVec2f& size, const ofxVec2f& divisions) {
		setDivisions(divisions);
		setSize(size);
	}
	void setDivisions(const ofxVec2f& divisions) {
		this->divisions = divisions;
		grid.resize((int) (divisions.x * divisions.y));
	}
	void setSize(const ofxVec2f& size) {
		this->size = size;
		int k = 0;
		for(int i = 0; i < divisions.y; i++) {
			float y = ofMap(i, 0, divisions.y - 1, 0, size.y);
			for(int j = 0; j < divisions.x; j++) {
				float x = ofMap(j, 0, divisions.x - 1, 0, size.x);
				grid[k++].setOrigin(x, y);
			}
		}
	}
	const ControlPoint& get(int x, int y) const {
		return grid[y * divisions.x + x];
	}
	const ofxVec2f& getDivisions() const {
		return divisions;
	}
	void update(const vector<ofxVec2f>& positions) {
		float forceShape = 1;
		float forceMagnitude = 2048;

		for(unsigned int i = 0; i < grid.size(); i++) {
			grid[i].update(positions, forceShape, forceMagnitude);
		}
	}
	void draw() const {
		glBegin(GL_POINTS);
		for(unsigned int i = 0; i < grid.size(); i++)
			grid[i].draw();
		glEnd();
	}
};
