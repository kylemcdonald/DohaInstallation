#pragma once

#include "ofxOsc.h"
#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void setupOsc();
	void update();
	void draw();

	float t;

    ofxVec2f curPoint;
	ofxOscSender oscSender;
};
