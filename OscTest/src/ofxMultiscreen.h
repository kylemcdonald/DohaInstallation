#pragma once

#include "ofxOsc.h"
#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"
#include "ofMain.h"

class MultiScreen {
public:
	float x, y;
	float width, height;
	string mode;

	MultiScreen() :
		x(0),
		y(0),
		width(0),
		height(0),
		mode("relative") {
	}
	MultiScreen(ofxXmlSettings& settings, MultiScreen defaultScreen, int which = 0) {
		x = settings.getAttribute("screen", "x", defaultScreen.x, which);
		y = settings.getAttribute("screen", "y", defaultScreen.y, which);
		width = settings.getAttribute("screen", "width", defaultScreen.width, which);
		height = settings.getAttribute("screen", "height", defaultScreen.height, which);
		mode = settings.getAttribute("screen", "mode", defaultScreen.mode, which);
	}
	bool relative() const {
		return mode.compare("relative") == 0;
	}
	float absoluteX() const {
		return (relative() ? width : 1) * x;
	}
	float absoluteY() const {
		return (relative() ? height : 1) * y;
	}

	friend ostream& operator<<(ostream& out, const MultiScreen& screen) {
		out << screen.width << "x" << screen.height << "@" << screen.x << "/" << screen.y << ":" << screen.mode;
		return out;
	}
};

class MultiWindow {
public:
	vector<MultiScreen> screens;

	MultiWindow() {
	}
	float getWidth() const {
		float width = 0;
		for(unsigned int i = 0; i < screens.size(); i++)
			width += screens[i].width;
		return width;
	}
	float getHeight() const {
		float height = 0;
		for(unsigned int i = 0; i < screens.size(); i++)
			height += screens[i].height;
		return height;
	}

	friend ostream& operator<<(ostream& out, const MultiWindow& window) {
		out << window.getWidth() << "x" << window.getHeight() << " { ";
		for(unsigned int i = 0; i < window.screens.size(); i++) {
			out << window.screens[i] << " ";
		}
		out << "}";
		return out;
	}
};

class MultiComputer {
public:
	string hostname;
	vector<MultiWindow> windows;

	MultiComputer(ofxXmlSettings& settings, int which = 0) {
		hostname = settings.getAttribute("computer", "hostname", "", which);
	}
	int execute(string command) {
		command = "ssh " + hostname + ".local \"" + command + "\"";
		return system(command.c_str());
	}
	void launch(string appName) {
		for(unsigned int i = 0; i < windows.size(); i++) {
			string window = ofToString((int) i);
			execute("export DISPLAY=:0." + window + "; " + appName + " " + window + " 0>/dev/null 1>&0 2>&0 &");
		}
	}

	friend ostream& operator<<(ostream& out, const MultiComputer& computer) {
		out << computer.hostname << "{" << computer.windows.size() << ": ";
		for(unsigned int i = 0; i < computer.windows.size(); i++) {
			out << computer.windows[i] << " ";
		}
		out << "}";
		return out;
	}
};

class ofxMultiscreen : public ofBaseApp {
public:
	static const string appName, appDirectory;

	static vector<MultiComputer> computers;
	static bool master;
	static string hostname;
	static int display;
	static MultiWindow window;

	static ofxOscSender oscSender;
	static ofxOscReceiver oscReceiver;

	static int maxScoutLines;

	static void multiSetup();
	static void loadScreens(ofxXmlSettings& settings);
	static void startScreens();
	static void stopScreens();
	static void execute(string command);
	static void launch(string appName);

	static string getHostname();
	static int getDisplay();

	~ofxMultiscreen();

	void draw();

	virtual void drawInsideViewport() = 0;
	virtual void drawOutsideViewport() = 0;
};
