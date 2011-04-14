#pragma once

#include "ofMain.h"
#include "VirtualCamera.h"
#include "ofxAutoControlPanel.h"
#include "ofxOpenCv.h"

class testApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);

		ofxAutoControlPanel panel;
		VirtualCamera cam;
		ofxCvGrayscaleImage gray;
		ofxCvContourFinder finder;
};
