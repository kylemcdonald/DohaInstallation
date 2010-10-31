#include "ofxMultiscreen.h"

// app is master by default, client by args
bool ofxMultiscreen::master = true;
int ofxMultiscreen::window = 0;
ofxVec2f ofxMultiscreen::windowSize(1920, 1080);
ofxVec2f ofxMultiscreen::offset(0, 0);
ofxOscSender ofxMultiscreen::oscSender;
ofxOscReceiver ofxMultiscreen::oscReceiver;

void ofxMultiscreen::multiSetup(int argc, char* argv[]) {
	if(argc > 1) {
		master = false;
		window = ofToInt(argv[1]);
		windowSize.set(1920, 1080);
	}

	ofxXmlSettings settings;
	settings.loadFile("settings.xml");

	if(master) {
		cout << "This computer is running as master." << endl;
	} else {
		cout << "This computer is running as a client on pipe " << window << "." << endl;
	}

	settings.pushTag("osc");
	string address = settings.getValue("address", "255.255.255.255");
	int port = settings.getValue("port", 8888);
	settings.popTag();

	if(master) {
		cout << "Connecting to " << address << ":" << port << endl;
		oscSender.setup(address, port);
	} else {
		cout << "Listening on port " << port << endl;
		oscReceiver.setup(port);
	}
}

void ofxMultiscreen::setup() {
	ofSetWindowPosition(0, 0);
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
	ofDrawBitmapString(scout.str(), 10, 20);
}
