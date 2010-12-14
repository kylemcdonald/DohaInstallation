#pragma once

#include "ofxMultiscreen.h"

#include "ofxOsc.h"
#include "ControlSurface.h"
#include "PhotoWall.h"
#include "PhotoManager.h"

class testApp : public ofxMultiscreen {
public:
	void setup();
	void setupOsc();
	void update();

	void drawLocal();
	void drawOverlay();

	ofxOscSender oscSender;
	ofxOscReceiver oscReceiver;

	ControlSurface surface;
	PhotoWall wall;
	
	float drawRadius;
	vector<ofxVec2f> forces;

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
};
