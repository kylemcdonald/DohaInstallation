#include "testApp.h"

void ofViewport(int left, int top, int width, int height, bool relative = true) {
	if(width == 0) {
		width = ofGetWidth();
	}
	if(height == 0) {
		height = ofGetHeight();
	}
	if(relative) {
		top = ofGetHeight() - (height + top);
	}
	glViewport(left, top, width, height);
}

void testApp::setup(){
	ofSetCircleResolution(128);
}

void testApp::update(){
}

void testApp::draw(){
	ofBackground(0);
	ofViewport(mouseX, mouseY, 128, 128, true);
	ofSetupScreenPerspective(256, 256);
	ofRect(1, 0, 255, 255);
	ofTranslate(-mouseX * 2, -mouseY * 2);
	drawSingle();
}

void testApp::drawSingle() {
	ofNoFill();
	ofSetColor(255);
	ofCircle(256, 256, 128);
	ofCircle(256, 256, 256);
	ofLine(256, 256, 256, 0);
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

