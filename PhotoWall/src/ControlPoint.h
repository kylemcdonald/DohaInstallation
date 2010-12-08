#pragma once

#include "ofxVectorMath.h"

class ControlPoint : public ofxVec2f {
protected:
	ofxVec2f origin;
public:
	ControlPoint() {
	}
	void setOrigin(float x, float y) {
		origin.set(x, y);
	}
	inline void update(const vector<ofxVec2f>& forceCenters, float forceShape, float forceMagnitude) {
		ofxVec2f position = origin;
		for(int i = 0; i < forceCenters.size(); i++) {
			ofxVec2f difference = origin - forceCenters[i];
			float length = difference.length();
			difference /= powf(length, forceShape);
			position += forceMagnitude * difference;
		}
		set(position);
	}
	inline void draw() const {
		glVertex2f(x, y);
	}
};