#pragma once

#include "ofxXmlSettings.h"
#include "ofxVectorMath.h"
#include "ofxOsc.h"
#include "ofMain.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void drawInsideViewport();
	void drawOutsideViewport();

	vector<ofPoint> points;

	ofxVec2f offset;

	static bool master;
	static int pipe;
	ofxOscSender oscSender;
	ofxOscReceiver oscReceiver;

	stringstream scout;

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
};
