#pragma once

#include "ofMain.h"
#include "ofxVirtualCamera.h"
#include "ofxAutoControlPanel.h"

class testApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);

		ofxAutoControlPanel panel;
		ofxVirtualCamera cam;
};
