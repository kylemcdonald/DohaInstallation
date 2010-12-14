#include "testApp.h"

void testApp::setup() {
    ofSetFrameRate(60);
	setupOsc();
}

void testApp::setupOsc() {
	ofxXmlSettings settings;
	if(!settings.loadFile("osc.xml")) {
		ofLog(OF_LOG_ERROR, "testApp::setupOsc(): couldn't load osc.xml");
	}
	string address = settings.getValue("address", "255.255.255.255");
	int port = settings.getValue("port", 8888);

    cout << "Broadcasting to " << address << ":" << port << endl;
    oscSender.setup(address, port);
}

void testApp::update() {
    t += ofGetLastFrameTime() * ofNoise(ofGetElapsedTimef()) * .1;
    curPoint.set(ofNoise(t), ofNoise(t + 100));

 	ofxOscMessage message;
    message.setAddress("mouse");

    message.addFloatArg(curPoint.x);
    message.addFloatArg(ofMap(curPoint.y, 0, 1, .8, .9));

    message.addFloatArg(curPoint.x);
    message.addFloatArg(curPoint.y);

    oscSender.sendMessage(message);
}

void testApp::draw() {
    ofBackground(0);
    ofSetColor(255);

    ofCircle(curPoint * ofxVec2f(ofGetWidth(), ofGetHeight()), 32);
    ofCircle(ofxVec2f(curPoint.x, .9) * ofxVec2f(ofGetWidth(), ofGetHeight()), 32);
}
