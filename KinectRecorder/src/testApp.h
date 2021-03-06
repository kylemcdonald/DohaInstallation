#pragma once

#include "ofMain.h"
#include "ofxKinect.h"

class testApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);

		ofxKinect kinect;
		
		bool recording;
		int curFrame, maxFrames;
		vector<ofImage*> images;
};
