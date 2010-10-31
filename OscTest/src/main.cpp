#include "testApp.h"

int main(int argc, char* argv[]) {
	if(argc > 1) {
		testApp::master = false;
		testApp::pipe = ofToInt(argv[1]);
	}

	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1920, 1080, OF_FULLSCREEN);
	ofRunApp( new testApp());
}
