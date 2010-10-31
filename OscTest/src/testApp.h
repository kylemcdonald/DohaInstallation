#pragma once

#include "ofxMultiscreen.h"

class testApp : public ofxMultiscreen {
public:
	void setup();
	void update();

	void drawInsideViewport();
	void drawOutsideViewport();

	vector<ofPoint> points;

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
};
