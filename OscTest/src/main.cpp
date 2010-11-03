#include "testApp.h"

int main() {
	testApp::multiSetup();
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, testApp::card.getWidth(), testApp::card.getHeight(), OF_FULLSCREEN);
	ofRunApp(new testApp());
}
