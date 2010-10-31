#include "testApp.h"

void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
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



