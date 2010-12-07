#pragma once

#include "ControlSurface.h"
#include "ControlPoint.h"

class Photo {
protected:
	const ControlPoint *nw, *ne, *sw, *se;
	float size;
	ofxVec2f position;
	float rotation;
	float brightness;
public:
	Photo();
	void setup(const ControlSurface& surface, float x, float y);
	void update();
	void draw() const;
	bool inside(const ofRectangle& window) const;
};
