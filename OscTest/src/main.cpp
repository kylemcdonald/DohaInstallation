#include "testApp.h"

int main(int argc, char* argv[]) {
	testApp::multiSetup(argc, argv);
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, testApp::windowSize.x, testApp::windowSize.y, OF_FULLSCREEN);
	ofRunApp(new testApp());
}
