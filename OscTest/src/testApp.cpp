#include "testApp.h"

void testApp::setup() {
	ofxXmlSettings settings, unique;
	settings.loadFile("settings.xml");
	unique.loadFile("unique.xml");

	ofSetLogLevel(OF_LOG_VERBOSE);

	master = unique.getValue("master", 0);
	if(master) {
		cout << "This computer is running as master." << endl;
	} else {
		cout << "This computer is running as a client." << endl;
	}

	unique.pushTag("viewport");
	offset.set(unique.getValue("x", 0), unique.getValue("y", 0));
	unique.popTag();

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

void testApp::update() {
	if(!master) {
		while(oscReceiver.hasWaitingMessages()) {
			ofxOscMessage message;
			oscReceiver.getNextMessage(&message);
			string address = message.getAddress();
			if(address.compare("mouse") == 0) {
				int mx = message.getArgAsInt32(0);
				int my = message.getArgAsInt32(1);
				ofPoint cur(mx, my);
				points.push_back(cur);

				scout << "got message at /" << address << " " << mx << " " << my << endl;
			} else if(address.compare("reset") == 0) {
				points.clear();

				scout << "got reset" << endl;
			}
		}
	}
}

void testApp::draw() {
	ofBackground(0, 0, 0);

	glPushMatrix();
	glTranslatef(-offset.x, -offset.y, 0);
	drawInsideViewport();
	glPopMatrix();

	drawOutsideViewport();
}

void testApp::drawInsideViewport() {
	ofSetColor(255, 255, 255);
	glBegin(GL_LINE_STRIP);
	for(unsigned int i = 0; i < points.size(); i++) {
		ofPoint& cur = points[i];
		glVertex2f(cur.x, cur.y);
	}
	glEnd();
}

void testApp::drawOutsideViewport() {
	if(master)
		ofSetColor(255, 0, 0);
	else
		ofSetColor(0, 255, 0);
	ofRect(0, 0, 64, 64);

	int checksum = 0;
	for(int i = 0; i < points.size(); i++) {
		ofPoint& cur = points[i];
		checksum ^= (int) cur.x;
		checksum ^= (int) cur.y;
	}

	ofSetColor(255, 255, 255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " " + ofToString(checksum), ofGetWidth() - 100, 20);
	ofDrawBitmapString(scout.str(), 10, 20);
}

void testApp::keyPressed(int key) {
}

void testApp::keyReleased(int key) {
}

void testApp::mouseMoved(int x, int y) {
	x *= 3;
	y *= 3;

	if(master) {
		ofxOscMessage message;
		message.setAddress("mouse");
		message.addIntArg(x);
		message.addIntArg(y);
		oscSender.sendMessage(message);
		scout << "sent message at /mouse " << " " << x << " " << y << endl;

		ofPoint cur(x, y);
		points.push_back(cur);
	}
}

void testApp::mouseDragged(int x, int y, int button) {
}

void testApp::mousePressed(int x, int y, int button) {
	if(master) {
		ofxOscMessage message;
		message.setAddress("reset");
		oscSender.sendMessage(message);

		points.clear();
	}
}

void testApp::mouseReleased(int x, int y, int button) {
}

void testApp::windowResized(int w, int h) {
}



