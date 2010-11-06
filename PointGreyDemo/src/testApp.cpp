#include "testApp.h"

void testApp::setup(){
	cameraManager.load("cameraSettings.xml");
}

void testApp::update() {
	cameraManager.update();
}

void testApp::draw(){
	ofBackground(0, 0, 0);

	cameraManager.draw();

	ofSetColor(255, 255, 255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
}


void testApp::keyPressed(int key){

}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y ){

}


void testApp::mouseDragged(int x, int y, int button){

}


void testApp::mousePressed(int x, int y, int button){

}


void testApp::mouseReleased(int x, int y, int button){

}


void testApp::windowResized(int w, int h){

}

