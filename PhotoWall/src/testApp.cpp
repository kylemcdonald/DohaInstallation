#include "testApp.h"

void testApp::setup() {
	ofSetWindowTitle(appName);

	ofSetLogLevel(OF_LOG_VERBOSE);

	drawRadius = 400;

	setupOsc();

	ofxVec2f size = getMaxSize();
	surface.setup(size, ofxVec2f(190, 190));
	wall.setup(surface);
}

void testApp::setupOsc() {
	ofxXmlSettings settings;
	if(!settings.loadFile("osc.xml")) {
		ofLog(OF_LOG_ERROR, "testApp::setupOsc(): couldn't load osc.xml");
	}
	string address = settings.getValue("address", "255.255.255.255");
	int port = settings.getValue("port", 8888);

	if(master) {
		cout << "Broadcasting to " << address << ":" << port << endl;
		oscSender.setup(address, port);
        startScreens();
	}

	cout << "Listening on port " << port << endl;
	oscReceiver.setup(port);
}

void testApp::update() {
	while(oscReceiver.hasWaitingMessages()) {
		ofxOscMessage message;
		oscReceiver.getNextMessage(&message);
		string address = message.getAddress();
		if(address.compare("mouse") == 0) {
			ofxVec2f maxSize = getMaxSize();
			vector<ofxVec2f> forces;
			for(int i = 0; i < message.getNumArgs(); i += 2) {
				float x = message.getArgAsFloat(i + 0);
				float y = message.getArgAsFloat(i + 1);
				ofxVec2f cur(x, y);
				cur *= maxSize;
				forces.push_back(cur);
			}

			surface.update(forces);
			wall.update();
		} else if(address.compare("debug") == 0) {
			ofxMultiscreen::debug = !ofxMultiscreen::debug;
		}
	}
}

void testApp::drawLocal() {
	ofBackground(0, 0, 0);
	glEnable(GL_DEPTH_TEST);

	ofxVec2f curPosition = localScreen.absolutePosition();
	ofxVec2f curSize = MultiScreen::size;
	curPosition -= drawRadius;
	curSize += drawRadius * 2;
	ofRectangle curWindow;
	curWindow.set(curPosition, curSize.x, curSize.y);

	wall.drawWindow(curWindow);
	glDisable(GL_DEPTH_TEST);
}

void testApp::drawOverlay() {
}

void testApp::keyPressed(int key) {
	if(key == 'f') {
		ofToggleFullscreen();
	}
}

void testApp::keyReleased(int key) {
}

void testApp::mouseMoved(int x, int y) {
	if(master) {
		ofxOscMessage message;
		message.setAddress("mouse");

		ofxVec2f cur(x, y);
		cur /= ofxVec2f(ofGetWidth(), ofGetHeight());

		message.addFloatArg(cur.x);
		message.addFloatArg(cur.y);
/*
		cur += .1;
		message.addFloatArg(cur.x);
		message.addFloatArg(cur.y);
*/
		oscSender.sendMessage(message);
	}
}

void testApp::mouseDragged(int x, int y, int button) {
}

void testApp::mousePressed(int x, int y, int button) {
	if(master) {
		ofxOscMessage message;
		message.setAddress("debug");
		oscSender.sendMessage(message);
	}
}

void testApp::mouseReleased(int x, int y, int button) {
}

void testApp::windowResized(int w, int h) {
}

