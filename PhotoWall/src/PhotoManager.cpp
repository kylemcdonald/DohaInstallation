#include "PhotoManager.h"

#include "ofMain.h"
#include "Poco/File.h"

vector<LazyImage*> PhotoManager::all;

void PhotoManager::setup(string path) {
	Poco::File resizedPath("~/Desktop/3rdiStream/resized/");
	
	vector<Poco::File> dateDirs;
	resizedPath.list(dateDirs);
	cout << "There are " << dateDirs.size() << " days available." << endl;
	
	for(unsigned int i = 0; i < dateDirs.size(); i++) {
		Poco::File& curDir = dateDirs[i];
		vector<Poco::File> curImages;
		curDir.list(curImages);
		
		for(unsigned int j = 0; j < curImages.size(); j++) {
			LazyImage* img = new LazyImage();
			img->setup(curImages[j].path());
			all.push_back(img);
		}	
	}
}

void PhotoManager::clear() {
	for(unsigned int i = 0; i < all.size(); i++) {
		delete all[i];
	}
	all.clear();
}

LazyImage* PhotoManager::randomPhoto() {
	return all[(int) ofRandom(0, all.size())];
}