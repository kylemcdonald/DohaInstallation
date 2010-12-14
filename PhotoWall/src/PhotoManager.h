#pragma once

#include "LazyImage.h"

class PhotoManager {
private:
	static vector<LazyImage*> all;
public:
	static void setup(string path);
	static void clear();
	static LazyImage* randomPhoto();
};
