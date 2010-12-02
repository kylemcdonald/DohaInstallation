#include "testApp.h"

int main() {
	// normally this runs with ofRunApp, but we need it sooner so we can load xml settings
#ifdef TARGET_OSX
	ofSetDataPathRoot("../../../data/");
#endif
	
	ofAppGlutWindow window;
	testApp::multiLoad();
	ofPoint size = testApp::card.getSize();
	cout << "Running at " << size.x << "x" << size.y << endl;
	ofSetupOpenGL(&window, size.x, size.y, OF_FULLSCREEN);
	testApp::multiSetup();
	ofRunApp(new testApp());
}
