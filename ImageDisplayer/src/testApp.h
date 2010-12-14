#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"

#define IMAGES_DIRECTORY "/Users/kyle/Desktop/3rdiStream/resized/"
#define UPDATE_DIRECTORY_RATE 5

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
	
	// wow, private section in ofBaseApp. that's a first.
	private:
	
		Poco::File currentDirectory;
		Poco::File currentFile;
		Poco::File parentDirectory;
		ofImage mostRecentImage;
		bool updateDirectory, currentDirAssigned;
		int currentDay;
		
		string dateFormat, timeFormat;
	
		int lastSeconds;
};

#endif
