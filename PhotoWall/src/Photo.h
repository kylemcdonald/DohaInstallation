#pragma once

#include "ControlSurface.h"
#include "ControlPoint.h"
#include "PhotoManager.h"
#include "ofxMultiscreen.h"

class Photo {
protected:
	static int photoWidth, photoHeight;

	const ControlPoint *nw, *ne, *sw, *se;
	float size;
	ofxVec2f position;
	float rotation;
	float brightness;

	ofColor baseColor;
	
	LazyImage* img;
public:
	void setup(const ControlSurface& surface, float x, float y);
	void update();
	void draw() const;
	bool inside(const ofRectangle& window) const;
};
