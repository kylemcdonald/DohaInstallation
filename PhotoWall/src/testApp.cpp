#include "testApp.h"

void testApp::setup() {
	ofSetWindowTitle(appName);
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);

	ofSeedRandom(0);
	PhotoManager::setup("~/Desktop/3rdiStream/resized/");
	
	setupOsc();

	ofxVec2f size = getMaxSize();
	surface.setup(size, ofxVec2f(60, 100));
	wall.setup(surface);
}

testApp::~testApp() {
	PhotoManager::clear();
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
			forces.clear();
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
	
	//glEnable(GL_DEPTH_TEST);
	ofxVec2f curPosition = localScreen.absolutePosition();
	ofxVec2f curSize = MultiScreen::size;
	ofRectangle curWindow;
	curWindow.set(curPosition, curSize.x, curSize.y);
	wall.drawWindow(curWindow);
	//glDisable(GL_DEPTH_TEST);
	
	if(ofxMultiscreen::debug) {
		for(unsigned int i = 0; i < forces.size(); i++) {
			ofSetColor(255);
			ofFill();
			ofCircle(forces[i], 1920);
		}
	}
}

void testApp::drawOverlay() {
}

void testApp::keyPressed(int key) {
	if(key == 'f') {
		ofToggleFullscreen();
	}
	if(key == ' ') {
		if(master) {
			ofxOscMessage message;
			message.setAddress("debug");
			oscSender.sendMessage(message);
		}
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
		
		oscSender.sendMessage(message);
	}
}

void testApp::mouseDragged(int x, int y, int button) {
}

void testApp::mousePressed(int x, int y, int button) {
}

void testApp::mouseReleased(int x, int y, int button) {
}

void testApp::windowResized(int w, int h) {
}

