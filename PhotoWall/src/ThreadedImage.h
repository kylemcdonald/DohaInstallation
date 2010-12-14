#pragma once

#include "ofMain.h"
#include "ofxThread.h"

class ThreadedImage : public ofImage, public ofxThread {
protected:
	bool lastCpu;
	bool loadedCpu, loadedGpu;
	string filename;
	void threadedFunction() {
		lastCpu = false;
		loadedCpu = false;
		loadedGpu = false;
		ofImage::loadImage(filename);
		loadedCpu = true;
	}
public:
	void loadImage(string filename) {
		this->filename = filename;
		setUseTexture(false);
		startThread();
	}
	const string& getFilename() {
		return filename;
	}
	void bind() {
		// consider changing this to if(loadedCpu) and then setUseTexture to false to unload every frame
		if(lastCpu != loadedCpu) {
			setUseTexture(true);
			tex.allocate(myPixels.width, myPixels.height, myPixels.glDataType);
			update();
		}
		lastCpu = loadedCpu;
		if(bUseTexture) {
			ofImage::bind();
		}
	}
};