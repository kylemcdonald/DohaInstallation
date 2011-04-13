#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxAutoControlPanel.h"

class testApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);

		ofxKinect kinect;
		vector<ofxVec3f> surface;
		ofxAutoControlPanel panel;
		vector<unsigned int> indices;
};
