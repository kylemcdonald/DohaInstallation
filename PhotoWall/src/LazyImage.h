#pragma once

#include "ThreadedImage.h"

class LazyImage : public ThreadedImage {
protected:
	bool lazyLoaded;
	inline void lazyLoad() {
		if(!lazyLoaded) {
			lazyLoaded = true;
			ThreadedImage::loadImage(filename);
		}
	}
public:
	void setup(string filename) {
		lazyLoaded = false;
		setUseTexture(true);
		this->filename = filename;
	}
	void bind() {
		lazyLoad();
		ThreadedImage::bind();
	}
};
