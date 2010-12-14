#include "testApp.h"


void testApp::setup()
{
	dateFormat = "%m-%d-%Y";
	timeFormat = "%H-%M-%S";

	currentDay = ofGetDay();
	updateDirectory = false;
	parentDirectory = IMAGES_DIRECTORY;
	updateDirectory = true;
	currentDirAssigned = false;

	// was causing lots of cpu usage
	ofSetFrameRate(10);
}


void testApp::update()
{

	int offset = 5;

	// not the fastest, but the clearest
	if( updateDirectory )
	{

		if( !currentDirAssigned )
		{
			vector<Poco::File> fileVec;
			parentDirectory.list(fileVec);
			if(fileVec.size() > 0)
			{
				currentDirAssigned = true;
				currentDirectory = fileVec[0];
			}
			else
			{
				return;
			}

		}


		Poco::DateTime currentDirTime;
		string foldername = currentDirectory.path().substr(currentDirectory.path().find_last_of("/")+1);
		Poco::DateTimeParser::parse(dateFormat, foldername, currentDirTime, offset);

		Poco::DirectoryIterator it(parentDirectory);
		Poco::DirectoryIterator end; // null
		while(it != end)
		{
			string name = it.name().substr(it.name().find_last_of("/")+1);
			Poco::DateTime date;
			Poco::DateTimeParser::parse( dateFormat, name, date, offset);
			if(date > currentDirTime)
			{
				currentDirTime = date;
			}
			++it;
		}

		currentDirectory = Poco::DateTimeFormatter::format(currentDirTime, dateFormat);
		// total hack b/c poco returns 1970 as 0970 and 2010 as 0010
		string corrected = currentDirectory.path();
		currentDirectory = parentDirectory.path() + "/" + corrected.replace( 6, 1, "2" );
		updateDirectory = false;

	}

	int curSeconds = ofGetSeconds();
	if(curSeconds != lastSeconds && curSeconds % UPDATE_DIRECTORY_RATE == 0)
	{
		cout << "updating directory " << currentDirectory.path() << endl;

		// check the day
		if(currentDay != ofGetDay()) updateDirectory = true;

		Poco::DirectoryIterator fit(currentDirectory);
		Poco::DirectoryIterator end;
		// now we have the newest directory, let's get the newest image

		Poco::DateTime imageFilenameTime;
		Poco::DateTimeParser::parse( timeFormat, currentFile.path(), imageFilenameTime, offset);

		while (fit != end)
		{
			Poco::DateTime time;
			Poco::DateTimeParser::parse(timeFormat, fit.name().substr(0, fit.name().find_last_of(".")), time, offset);
			if(time > imageFilenameTime)
			{
				imageFilenameTime = time;
				currentFile = *fit;
			}
			++fit;
		}

		currentFile = Poco::DateTimeFormatter::format(imageFilenameTime, timeFormat);
		string appended = currentFile.path()+".jpg"; // sleepy
		string imgPath = currentDirectory.path()+"/"+appended;
		cout << "loading image from " << imgPath << endl;
		mostRecentImage.loadImage(imgPath);
	}
	lastSeconds = curSeconds;
}


void testApp::draw()
{
	// span horizontally until filled
	float w = mostRecentImage.getWidth();
	float h = mostRecentImage.getHeight();
	float s = ofGetWidth() / w;
	mostRecentImage.draw(0, 0, w * s, h * s);
}


void testApp::keyPressed(int key){
	//--currentDay;
}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y ){

}


void testApp::mouseDragged(int x, int y, int button){

}


void testApp::mousePressed(int x, int y, int button){

}


void testApp::mouseReleased(int x, int y, int button){

}


void testApp::windowResized(int w, int h){

}

