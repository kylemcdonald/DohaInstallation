#include "testApp.h"

int main() {
	testApp::multiSetup();
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, testApp::window.getWidth(), testApp::window.getHeight(), OF_FULLSCREEN);
	ofRunApp(new testApp());
}
