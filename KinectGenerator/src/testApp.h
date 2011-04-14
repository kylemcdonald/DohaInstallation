#pragma once

#include "ofMain.h"
#include "ofxVirtualCamera.h"
#include "ofxAutoControlPanel.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void setupOsc();
	void update();
	void updateOsc();
	void draw();
	void keyPressed(int key);
	
	ofxAutoControlPanel panel;
	ofxVirtualCamera cam;
	ofxCvGrayscaleImage blur, thresh;
	ofxCvContourFinder finder;
	ofxOscSender oscSender;
};
