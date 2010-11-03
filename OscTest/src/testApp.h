#pragma once

#include "ofxOsc.h"
#include "ofxMultiscreen.h"

class testApp : public ofxMultiscreen {
public:
	void setup();
	void update();

	void drawLocal();
	void drawOverlay();

	vector<ofPoint> points;

	ofxOscSender oscSender;
	ofxOscReceiver oscReceiver;

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
};
