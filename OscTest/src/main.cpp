#include "testApp.h"

int main() {
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1920, 1080, OF_FULLSCREEN);
	ofRunApp( new testApp());
}
