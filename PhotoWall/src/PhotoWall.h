#pragma once

#include "ControlSurface.h"
#include "Photo.h"

class PhotoWall {
protected:
	vector<Photo> photos;
public:
	PhotoWall();
	void setup(const ControlSurface& surface);
	void update();
	void draw() const;
	void drawWindow(const ofRectangle& window) const;
};
