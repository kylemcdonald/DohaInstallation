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
		resize(816 / 2, 612 / 2);
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
			#if OF_VERSION==6
			tex.allocate(getWidth(), getHeight(), myPixels.glDataType);
			#else
			tex.allocate(getWidth(), getHeight(), pixels.getGlDataType());
			#endif
			update();
		}
		lastCpu = loadedCpu;
		if(bUseTexture) {
			ofImage::bind();
		}
	}
};