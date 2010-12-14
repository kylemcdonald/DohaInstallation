#pragma once

#include "ofMain.h"

class LazyImage : public ofImage {
protected:
	string filename;
	bool loaded;
public:
	void setup(string filename) {
		this->filename = filename;
		loaded = false;
	}
	inline void load() {
		if(!loaded) {
			setUseTexture(true);
			loadImage(filename);
			loaded = true;
		}
	}
	void bind() {
		load();
		ofImage::bind();
	}
};
