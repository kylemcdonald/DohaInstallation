#include "testApp.h"

void testApp::setup() {	
	ofSetWindowTitle(appName);
		
	ofSetLogLevel(OF_LOG_VERBOSE);

	setupOsc();

	ofxVec2f size = getMaxSize();
	surface.setup(size, ofxVec2f(40, 35));
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
			int mx = message.getArgAsInt32(0);
			int my = message.getArgAsInt32(1);
			surface.update(ofxVec2f(mx, my));
			wall.update();
		} else if(address.compare("debug") == 0) {
			ofxMultiscreen::debug = !ofxMultiscreen::debug;
		}
	}
}

void testApp::drawLocal() {
	ofBackground(0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	wall.draw();
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
	x *= 14;
	y *= 14;

	if(master) {
		ofxOscMessage message;
		message.setAddress("mouse");
		message.addIntArg(x);
		message.addIntArg(y);
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

