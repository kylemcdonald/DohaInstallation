#include "testApp.h"

string testApp::dateFormat = "%m-%d-%Y";
string testApp::timeFormat = "%H-%M-%S";

void testApp::setup() {
	ofSetFrameRate(10);
	lastSeconds = 0;
}

Poco::Path testApp::getMostRecent(Poco::Path path, const string& format) {
	int offset = 0;
	Poco::DirectoryIterator itr(path), end;
	Poco::DateTime bestTime;
	Poco::Path bestPath;
	bool hasPath = false;
	while(itr != end) {
		Poco::Path curPath = itr.path();
		Poco::DateTime curTime = Poco::DateTimeParser::parse(format, curPath.getBaseName(), offset);
		if(!hasPath || curTime > bestTime) {
			bestTime = curTime;
			bestPath = curPath;
			hasPath = true;
		}
		++itr;
	}
	return bestPath;
}

void testApp::update() {
	int curSeconds = ofGetSeconds();
	if(curSeconds != lastSeconds && curSeconds % UPDATE_DIRECTORY_RATE == 0) {
		Poco::Path currentDirectory = getMostRecent(IMAGES_DIRECTORY, dateFormat);	
		Poco::Path currentImage = getMostRecent(currentDirectory, timeFormat);
		string curFilename = currentImage.toString();
		cout << "Loading image from " << curFilename << endl;
		// sleep for half a cycle to let the image download
		ofSleepMillis(UPDATE_DIRECTORY_RATE * 1000 / 2);
		mostRecentImage.loadImage(curFilename);
	}
	lastSeconds = curSeconds;
}


void testApp::draw() {
	// span horizontally until filled
	float w = mostRecentImage.getWidth();
	float h = mostRecentImage.getHeight();
	float s = ofGetWidth() / w;
	mostRecentImage.draw(0, 0, w * s, h * s);
}
