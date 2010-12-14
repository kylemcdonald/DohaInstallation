#pragma once

#include "ThreadedImage.h"

class LazyImage : public ThreadedImage {
protected:
	bool lazyLoaded;
	inline void lazyLoad() {
		if(!lazyLoaded) {
			setUseTexture(true);
			loadImage(filename);
			lazyLoaded = true;
		}
	}
public:
	void setup(string filename) {
		lazyLoaded = false;
		this->filename = filename;
	}
	void bind() {
		/*
		lazyLoad();
		if(loaded) {
			ofImage::bind();
		}
		*/
	}
};
