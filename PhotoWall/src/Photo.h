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
	Photo() {
	}
	void setup(const ControlSurface& surface, float x, float y) {
		nw = &(surface.get(floor(x), floor(y)));
		ne = &(surface.get(ceil(x), floor(y)));
		sw = &(surface.get(floor(x), ceil(y)));
		se = &(surface.get(ceil(x), ceil(y)));
	}
	inline void update() {
		size = nw->distance(*se) + ne->distance(*sw);
		size /= 4;

		position = *nw + *ne + *sw + *se;
		position /= 4;

		ofxVec2f o1 = *sw - *se;
		ofxVec2f o2 = *nw - *ne;
		rotation = atan2f(o1.y, o1.x) + atan2f(o2.y, o2.x);
		rotation /= 2;

		brightness = ofMap(size, 100, 200, 1, 0, true);
	}
	inline void draw() const {
		glPushMatrix();
		
		glTranslatef(position.x, position.y, 0);
		ofRotate(ofRadToDeg(rotation));
		
		// frame
		glColor3f(1, 1, 1);
		glScalef(size * 4 / 3, size, 1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(-.5, -.5);
		glVertex2f(-.5, .5);
		glVertex2f(.5, .5);
		glVertex2f(.5, -.5);
		glEnd();

		// photo
		glColor3f(brightness, brightness, brightness);
		glScalef(.95, .95, 1);
		glBegin(GL_QUADS);
		glVertex2f(-.5, -.5);
		glVertex2f(-.5, .5);
		glVertex2f(.5, .5);
		glVertex2f(.5, -.5);
		glEnd();
 
		glPopMatrix();
	}
};
