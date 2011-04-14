#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxFbo.h"

class VirtualCamera {
public:
	VirtualCamera();
	~VirtualCamera();
	
	void setup();
	void update();
	bool isFrameNew();
	void draw(float x, float y);
	unsigned char* getPixels();
	
	void setMaxLen(float maxLen);
	void setStepSize(int stepSize);
	void setClipping(float nearClipping, float farClipping);
	void setOrthoScale(float orthoScale);
	void setPosition(ofxVec3f position);
	void setRotation(ofxVec3f rotation);
private:
	ofxKinect kinect;
	
	vector<ofxVec3f> surface;
	vector<unsigned int> indices;
	
	ofxFbo fbo;
	ofImage fboPixels;
	
	bool newFrame;
	
	float maxLen;
	int stepSize;
	float nearClipping, farClipping;
	float orthoScale;
	ofxVec3f position, rotation;
};