#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"

#define IMAGES_DIRECTORY "/Users/base/Desktop/images/"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
	private:
	
		Poco::File currentDirectory;
		Poco::File currentFile;
		Poco::File parentDirectory;
		ofImage mostRecentImage;
		bool updateDirectory, currentDirAssigned;
		int currentDay;
		
		string dateFormat, timeFormat;

};

#endif
