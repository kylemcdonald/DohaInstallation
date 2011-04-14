#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	
	panel.setup("Control Panel", 5, 5, 280, 600);
	panel.addPanel("Camera");
	panel.addSlider("maxLen", "maxLen", 8, 0, 20);
	panel.addSlider("stepSize", "stepSize", 1, 1, 10, true);
	panel.addSlider("nearClipping", "nearClipping", 380, 0, 1024);
	panel.addSlider("farClipping", "farClipping", 600, 0, 1024);
	panel.addSlider("orthoScale", "orthoScale", .66, 0, 2);
	panel.addSlider("camx", "camx", 0, -1024, 1024);
	panel.addSlider("camy", "camy", -240, -1024, 1024);
	panel.addSlider("camz", "camz", -500, -1024, 1024);
	panel.addSlider("camrx", "camrx", 90, -180, 180);
	panel.addSlider("camry", "camry", 0, -180, 180);
	panel.addSlider("camrz", "camrz", 0, -180, 180);
	
	cam.setup();
}

//--------------------------------------------------------------
void testApp::update() {
	cam.setMaxLen(panel.getValueF("maxLen"));
	cam.setStepSize(panel.getValueF("stepSize"));
	cam.setClipping(panel.getValueF("nearClipping"), panel.getValueF("farClipping"));
	cam.setOrthoScale(panel.getValueF("orthoScale"));
	cam.setPosition(ofxVec3f(panel.getValueF("camx"), panel.getValueF("camy"), panel.getValueF("camz")));
	cam.setRotation(ofxVec3f(panel.getValueF("camrx"), panel.getValueF("camry"), panel.getValueF("camrz")));
	
	cam.update();
	if(cam.isFrameNew()) {
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(0, 0, 0);
	ofSetColor(255, 255, 255);
	cam.draw(0, 0);
}

void testApp::keyPressed(int key) {
}