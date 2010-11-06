#include "testApp.h"

int main(){
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, CameraManager::camWidth * 2, CameraManager::camHeight, OF_WINDOW);
	ofRunApp(new testApp());
}
