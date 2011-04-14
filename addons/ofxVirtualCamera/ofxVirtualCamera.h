#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxFbo.h"

class ofxVirtualCamera {
public:
	ofxVirtualCamera();
	~ofxVirtualCamera();
	
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
	
	int getWidth() const;
	int getHeight() const;
private:
	ofxKinect kinect;
	
	vector<ofxVec3f> surface;
	vector<unsigned int> indices;
	
	ofxFbo fbo;
	ofImage fboColorImage;
	ofImage fboGrayImage;
	
	bool newFrame;
	
	float maxLen;
	int stepSize;
	float nearClipping, farClipping;
	float orthoScale;
	ofxVec3f position, rotation;
	
	void updateSurface();
	void updateMesh();
	void renderCamera();
	void updatePixels();
};