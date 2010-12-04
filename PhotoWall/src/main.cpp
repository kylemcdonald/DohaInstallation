#include "testApp.h"

int main() {
	ofAppGlutWindow window;
	testApp::multiLoad();
	ofPoint size = testApp::card.getSize();
	cout << "Running at " << size.x << "x" << size.y << endl;
	ofSetupOpenGL(&window, size.x, size.y, OF_FULLSCREEN);
	testApp::multiSetup();
	ofRunApp(new testApp());
}
