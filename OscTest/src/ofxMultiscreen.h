#pragma once

#include "ofxOsc.h"
#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"
#include "ofMain.h"

class MultiScreen {
public:
	float x, y;
	float width, height;
	int mode;

	MultiScreen() :
	x(0),
	y(0),
	width(0),
	height(0),
	mode(1) {
	}
	MultiScreen(ofxXmlSettings& settings, MultiScreen defaultScreen, int which = 0) {
		x = settings.getAttribute("screen", "x", defaultScreen.x, which);
		y = settings.getAttribute("screen", "y", defaultScreen.y, which);
		width = settings.getAttribute("screen", "width", defaultScreen.width, which);
		height = settings.getAttribute("screen", "height", defaultScreen.height, which);
		mode = settings.getAttribute("screen", "mode", defaultScreen.mode, which);
	}
};

class MultiWindow {
public:
	float width, height;
	int mode;
	vector<MultiScreen> screens;

	MultiWindow() :
	width(0),
	height(0),
	mode(1) {
	}
	MultiWindow(ofxXmlSettings& settings, MultiWindow defaultWindow, int which = 0) {
		width = settings.getAttribute("window", "width", defaultWindow.width, which);
		height = settings.getAttribute("window", "height", defaultWindow.height, which);
		mode = settings.getAttribute("window", "mode", defaultWindow.mode, which);
	}
};

class MultiComputer {
public:
	string address;
	vector<MultiWindow> windows;

	MultiComputer(ofxXmlSettings& settings, int which = 0) {
		address = settings.getAttribute("computer", "address", "", which);
	}
};

class ofxMultiscreen : public ofBaseApp {
public:
	static bool master;
	static int window;
	static ofxVec2f windowSize;
	static ofxVec2f offset;

	static vector<MultiComputer> computers;

	static ofxOscSender oscSender;
	static ofxOscReceiver oscReceiver;

	static int maxScoutLines;

	static void multiSetup(int argc, char* argv[]);
	static void loadScreens(ofxXmlSettings& settings);
	static void startScreens();

	void draw();

	virtual void drawInsideViewport() = 0;
	virtual void drawOutsideViewport() = 0;
};
