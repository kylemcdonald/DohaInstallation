#include "testApp.h"

int main() {
	ofAppGlutWindow window;
	testApp::multiLoad();
	ofSetupOpenGL(&window, testApp::card.getWidth(), testApp::card.getHeight(), OF_FULLSCREEN);
	testApp::multiSetup();
	ofRunApp(new testApp());
}
