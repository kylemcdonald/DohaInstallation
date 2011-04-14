#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	
	panel.setup("Control Panel", 5, 5, 280, 600);
	panel.addPanel("Contours");
	panel.addSlider("threshLevel", "threshLevel", 128, 0, 255, true);
	panel.addSlider("minArea", "minArea", 0, 0, 50, true);
	panel.addSlider("maxArea", "maxArea", 240 * 240, 32 * 32, 240 * 240, true);
	panel.addSlider("nConsidered", "nConsidered", 8, 1, 16, true);
	
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
	
	gray.allocate(640, 480);
}

//--------------------------------------------------------------
void testApp::update() {
	cam.setMaxLen(panel.getValueF("maxLen"));
	cam.setStepSize(panel.getValueF("stepSize"));
	cam.setClipping(panel.getValueF("nearClipping"), panel.getValueF("farClipping"));
	cam.setOrthoScale(panel.getValueF("orthoScale"));
	cam.setPosition(ofxVec3f(panel.getValueF("camx"), panel.getValueF("camy"), panel.getValueF("camz")));
	cam.setRotation(ofxVec3f(panel.getValueF("camrx"), panel.getValueF("camry"), panel.getValueF("camrz")));
	
	int threshLevel = panel.getValueI("threshLevel");
	int minArea = panel.getValueI("minArea");
	int maxArea = panel.getValueI("maxArea");
	int nConsidered = panel.getValueI("nConsidered");
	
	cam.update();
	if(cam.isFrameNew()) {
		gray.setFromPixels(cam.getPixels(), 640, 480);
		gray.threshold(threshLevel);
		finder.findContours(gray, minArea, maxArea, nConsidered, false);
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(0, 0, 0);
	ofSetColor(255, 255, 255);
	cam.draw(0, 0);
	gray.draw(640, 0);
	finder.draw(0, 0);
}

void testApp::keyPressed(int key) {
}