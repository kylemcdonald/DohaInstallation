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
	inline void update(const ofxVec2f& forceCenter, float forceShape, float forceMagnitude) {
		ofxVec2f difference = origin - forceCenter;
		float length = difference.length();
		difference /= powf(length, forceShape);
		ofxVec2f position = origin + forceMagnitude * difference;
		set(position);
	}
	inline void draw() const {
		glVertex2f(x, y);
	}
};