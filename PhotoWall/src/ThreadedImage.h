#pragma once

#include "ofMain.h"
#include "ofxThread.h"

class ThreadedImage : public ofImage, public ofxThread {
protected:
	bool loaded;
	string filename;
	void threadedFunction() {
		loaded = false;
		ofImage::loadImage(filename);
		loaded = true;
	}
public:
	void loadImage(string filename) {
		this->filename = filename;
		startThread();
	}
};