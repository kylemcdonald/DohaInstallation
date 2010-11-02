#include "ofxMultiscreen.h"

const string ofxMultiscreen::appName = "OscTest";
const string ofxMultiscreen::appDirectory = "~/Desktop/openFrameworks/apps/DohaInstallation";

// app is master by default, client by args
bool ofxMultiscreen::master = true;
int ofxMultiscreen::window = 0;
ofxVec2f ofxMultiscreen::windowSize(1920, 1080);
ofxVec2f ofxMultiscreen::offset(0, 0);
ofxOscSender ofxMultiscreen::oscSender;
ofxOscReceiver ofxMultiscreen::oscReceiver;
vector<MultiComputer> ofxMultiscreen::computers;

void ofxMultiscreen::multiSetup(int argc, char* argv[]) {
	if(argc > 1) {
		master = false;
		window = ofToInt(argv[1]);
	}

	ofxXmlSettings settings;
	settings.loadFile("settings.xml");

	if(master) {
		cout << "This computer is running as master." << endl;
	} else {
		cout << "This computer is running as a client on pipe " << window << "." << endl;
	}

	settings.pushTag("osc");\
	string address = settings.getValue("address", "255.255.255.255");
	int port = settings.getValue("port", 8888);
	settings.popTag();

	loadScreens(settings);
	if(master) {
		cout << "Connecting to " << address << ":" << port << endl;
		oscSender.setup(address, port);
		startScreens();
	} else {
		cout << "Listening on port " << port << endl;
		oscReceiver.setup(port);
		for(unsigned int i = 0; i < computers.size(); i++) {
		}
	}
}

void ofxMultiscreen::loadScreens(ofxXmlSettings& settings) {
	settings.pushTag("defaults");
	MultiScreen defaultScreen(settings, MultiScreen());
	settings.popTag();

	settings.pushTag("computers");
	int nComputers = settings.getNumTags("computer");
	for(int whichComputer = 0; whichComputer < nComputers; whichComputer++) {
		MultiComputer computer(settings, whichComputer);
		settings.pushTag("computer", whichComputer);
		int nWindows = settings.getNumTags("window");
		for(int whichWindow = 0; whichWindow < nWindows; whichWindow++) {
			MultiWindow window;
			settings.pushTag("window", whichWindow);
			int nScreens = settings.getNumTags("screen");
			for(int whichScreen = 0; whichScreen < nScreens; whichScreen++) {
				MultiScreen screen(settings, defaultScreen, whichScreen);
				window.screens.push_back(screen);
			}
			settings.popTag();
			computer.windows.push_back(window);
		}
		settings.popTag();
		computers.push_back(computer);
	}
	settings.popTag();

	if(ofLogLevel() == OF_LOG_VERBOSE) {
		cout << "Loaded " << computers.size() << " computers:" << endl;
		for(unsigned int i = 0; i < computers.size(); i++) {
			cout << computers[i] << endl;
		}
	}
}

void ofxMultiscreen::startScreens() {
	launch("cd " + appDirectory + "/" + appName + "/bin; ./" + appName);
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
	glTranslatef(-offset.x, -offset.y, 0);
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
	stopScreens();
}
