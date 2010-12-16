#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"

#define IMAGES_DIRECTORY "/home/kyle/Desktop/3rdiStream/resized/"
#define UPDATE_DIRECTORY_RATE 5

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

private:
	static string dateFormat, timeFormat;
	
	ofImage mostRecentImage;
	int lastSeconds;
	
	Poco::Path getMostRecent(Poco::Path path, const string& format);
};

#endif
