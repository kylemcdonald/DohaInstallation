#pragma once

#include "ofxOsc.h"
#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"
#include "ofMain.h"

class ofxMultiscreen : public ofBaseApp {
public:

	static bool master;
	static int window;
	static ofxVec2f windowSize;
	static ofxVec2f offset;

	static ofxOscSender oscSender;
	static ofxOscReceiver oscReceiver;

	stringstream scout;

	static void multiSetup(int argc, char* argv[]);

	virtual void setup();
	void draw();

	virtual void drawInsideViewport() = 0;
	virtual void drawOutsideViewport() = 0;
};
