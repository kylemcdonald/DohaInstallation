#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();
	
	recording = false;
	maxFrames = 30 * 15;
	curFrame = 0;
	
	images.resize(maxFrames);
	for(int i = 0; i < maxFrames; i++) {
		images[i] = new ofImage();
		images[i]->allocate(640, 480, OF_IMAGE_GRAYSCALE);
		images[i]->setUseTexture(false);
	}

	kinect.getCalibration().setClippingInCentimeters(100, 500);
}

//--------------------------------------------------------------
void testApp::update()
{
	ofBackground(100, 100, 100);

	kinect.update();
	if(kinect.isFrameNew() && recording) {
		cout << "recording frame " << curFrame << "/" << maxFrames << endl;
		
		memcpy(images[curFrame]->getPixels(), kinect.getDepthPixels(), 640 * 480);
		curFrame++;
		
		if(curFrame >= maxFrames) {
			for(int i = 0; i < maxFrames; i++) {
				images[i]->saveImage(ofToString(1000 + i) + ".png");
			}
			
			recording = false;
		}
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofSetColor(255, 255, 255);
	kinect.drawDepth(0, 0, 640, 480);
}

//--------------------------------------------------------------
void testApp::exit() {
	kinect.close();
}

void testApp::keyPressed(int key) {
	if(key == '\t') {
		recording = !recording;
	}
}