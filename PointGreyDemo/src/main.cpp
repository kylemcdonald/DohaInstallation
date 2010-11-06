#include "testApp.h"

int main(){
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, testApp::camWidth * 2, testApp::camHeight, OF_WINDOW);
	ofRunApp(new testApp());
}
