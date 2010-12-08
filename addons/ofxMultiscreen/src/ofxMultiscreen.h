#pragma once

#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"
#include "ofMain.h"

#include "MultiComputer.h"
#include "MultiScreen.h"
#include "MultiCard.h"

class ofxMultiscreen : public ofBaseApp {
public:
	static string appName, appDirectory;

	static bool powersave;

	static vector<MultiComputer> computers;
	static bool master;
	static string hostname;
	static int display;
	static MultiCard card;
	static MultiScreen localScreen;
	static ofTrueTypeFont font;
	static bool debug;

	static void multiLoad(); // call before setting up OpenGL
	static void multiSetup(); // call after setting up OpenGL

	static void loadScreens(ofxXmlSettings& settings);
	static void startScreens();
	static void stopScreens();
	static void execute(string command);
	static void executeDisplay(string command);
	static void launch(string appName);

	static string getHostname();
	static int getDisplay();

	~ofxMultiscreen();

	void draw();
	void drawScreen();
	void drawDebug();

	virtual void drawLocal() = 0;
	virtual void drawOverlay() = 0;

	static float ofGetWidthLocal();
	static float ofGetHeightLocal();

	static ofPoint maxSize;
	static bool hasMaxSize;
	static ofPoint getMaxSize();
};
