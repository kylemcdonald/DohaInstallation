#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"

#include "FlyCapture2.h"
using namespace FlyCapture2;

class testApp : public ofBaseApp{
public:
	void setup();
	~testApp();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	vector<Camera*> cameras;
	ofImage curImage;
};
