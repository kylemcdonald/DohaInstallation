#include "ofxMultiscreen.h"

const string ofxMultiscreen::appName = "OscTest";
const string ofxMultiscreen::appDirectory = "~/Desktop/openFrameworks/apps/DohaInstallation/OscTest/bin";

bool ofxMultiscreen::master = true; // app is master by default
int ofxMultiscreen::display = 0;
string ofxMultiscreen::hostname;
MultiWindow ofxMultiscreen::window;
vector<MultiComputer> ofxMultiscreen::computers;

void ofxMultiscreen::multiSetup() {
	ofxXmlSettings settings;
	settings.loadFile("settings.xml");

	loadScreens(settings);

	if(ofLogLevel() == OF_LOG_VERBOSE) {
		if(master) {
			cout << "This computer is running as master." << endl;
		} else {
			cout << "This computer is running as a client." << endl;
		}
		cout << "Running on computer " << hostname << " on display " << display << endl;
	}
}

void ofxMultiscreen::loadScreens(ofxXmlSettings& settings) {
	settings.pushTag("defaults");
	MultiScreen defaultScreen(settings, MultiScreen());
	settings.popTag();

	hostname = getHostname();
	display = getDisplay();

	settings.pushTag("computers");
	int nComputers = settings.getNumTags("computer");
	for(int whichComputer = 0; whichComputer < nComputers; whichComputer++) {
		MultiComputer curComputer(settings, whichComputer);
		settings.pushTag("computer", whichComputer);
		int nWindows = settings.getNumTags("window");
		for(int whichWindow = 0; whichWindow < nWindows; whichWindow++) {
			MultiWindow curWindow;
			settings.pushTag("window", whichWindow);
			int nScreens = settings.getNumTags("screen");
			for(int whichScreen = 0; whichScreen < nScreens; whichScreen++) {
				MultiScreen screen(settings, defaultScreen, whichScreen);
				curWindow.screens.push_back(screen);
			}
			settings.popTag();
			curComputer.windows.push_back(curWindow);

			if(whichWindow == display && curComputer.hostname.compare(hostname) == 0) {
				master = false;
				window = curWindow;
			}
		}
		settings.popTag();
		computers.push_back(curComputer);
	}
	settings.popTag();

	if(ofLogLevel() == OF_LOG_VERBOSE) {
		for(unsigned int i = 0; i < computers.size(); i++) {
			cout << computers[i] << endl;
		}
	}
}

void ofxMultiscreen::startScreens() {
	launch("cd " + appDirectory + "; ./" + appName);
}

void ofxMultiscreen::stopScreens() {
	execute("killall -9 " + appName);
}

void ofxMultiscreen::execute(string command) {
	for(unsigned int i = 0; i < computers.size(); i++) {
		computers[i].execute(command);
	}
}

void ofxMultiscreen::launch(string appName) {
	for(unsigned int i = 0; i < computers.size(); i++) {
		computers[i].launch(appName);
	}
}

void ofxMultiscreen::draw() {
	ofBackground(0, 0, 0);

	glPushMatrix();
	if(!master)
		glTranslatef(-window.screens[0].absoluteX(), -window.screens[0].absoluteY(), 0);
	drawInsideViewport();
	glPopMatrix();

	glPushMatrix();
	drawOutsideViewport();
	glPopMatrix();

	if(master)
		ofSetColor(255, 0, 0);
	else
		ofSetColor(0, 255, 0);
	ofRect(0, 0, 64, 64);

	ofSetColor(255, 255, 255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 60, ofGetHeight() - 10);
}

ofxMultiscreen::~ofxMultiscreen() {
	if(master)
		stopScreens();
}

#define MAX_HOSTNAME_LENGTH 256
string ofxMultiscreen::getHostname() {
	char hostname[MAX_HOSTNAME_LENGTH];
	gethostname(hostname, MAX_HOSTNAME_LENGTH);
	return hostname;
}

int ofxMultiscreen::getDisplay() {
	string display(getenv("DISPLAY"));
	vector<string> parts = ofSplitString(display, ".");
	return ofToInt(parts[1]);
}
