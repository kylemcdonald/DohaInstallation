#include "testApp.h"

void catchError(Error error) {
	if(error != PGRERROR_OK)
		error.PrintErrorTrace();
}

void PrintCameraInfo( CameraInfo* pCamInfo) {
	printf(
		"\n*** CAMERA INFORMATION ***\n"
		"Serial number - %u\n"
		"Camera model - %s\n"
		"Camera vendor - %s\n"
		"Sensor - %s\n"
		"Resolution - %s\n"
		"Firmware version - %s\n"
		"Firmware build time - %s\n\n",
		pCamInfo->serialNumber,
		pCamInfo->modelName,
		pCamInfo->vendorName,
		pCamInfo->sensorInfo,
		pCamInfo->sensorResolution,
		pCamInfo->firmwareVersion,
		pCamInfo->firmwareBuildTime);
}


void testApp::setup(){
	ofxXmlSettings cameraSettings;
	cameraSettings.loadFile("cameraSettings.xml");

	BusManager busMgr;
	unsigned int numCameras;
	catchError(busMgr.GetNumOfCameras(&numCameras));
	for(unsigned int i = 0; i < numCameras; i++) {
		Camera* camera = new Camera();

		PGRGuid guid;
		CameraInfo camInfo;

		catchError(busMgr.GetCameraFromIndex(i, &guid));
		catchError(camera->Connect(&guid));
		catchError(camera->GetCameraInfo(&camInfo));
		PrintCameraInfo(&camInfo);

		catchError(camera->SetVideoModeAndFrameRate(VIDEOMODE_640x480Y8, FRAMERATE_60));
		catchError(camera->StartCapture());

		Property brightness(BRIGHTNESS);
		brightness.autoManualMode = false;
		brightness.valueA = cameraSettings.getValue("brightness", 1);
		catchError(camera->SetProperty(&brightness));

		Property autoExposure(AUTO_EXPOSURE);
		autoExposure.onOff = true;
		autoExposure.autoManualMode = false;
		autoExposure.valueA = cameraSettings.getValue("exposure", 7);
		catchError(camera->SetProperty(&autoExposure));

		Property gamma(GAMMA);
		gamma.onOff = false;
		catchError(camera->SetProperty(&gamma));

		Property shutter(SHUTTER);
		shutter.valueA = cameraSettings.getValue("shutter", 531);
		shutter.autoManualMode = false;
		catchError(camera->SetProperty(&shutter));

		Property gain(GAIN);
		gain.valueA = cameraSettings.getValue("gain", 16);
		gain.autoManualMode = false;
		catchError(camera->SetProperty(&gain));

		cameras.push_back(camera);

		ofxCvGrayscaleImage* image = new ofxCvGrayscaleImage();
		image->allocate(camWidth, camHeight);
		images.push_back(image);
	}
}

testApp::~testApp() {
	for(unsigned int i = 0; i < cameras.size(); i++) {
		cameras[i]->StopCapture();
		cameras[i]->Disconnect();
		delete cameras[i];
		delete images[i];
	}
}

void testApp::update() {
}


void testApp::draw(){
	ofBackground(0, 0, 0);

	glPushMatrix();
	for(unsigned int i = 0; i < cameras.size(); i++) {
		Camera& camera = *(cameras[i]);
		Image rawImage;
		catchError(camera.RetrieveBuffer(&rawImage));

		ofxCvGrayscaleImage& curImage = *(images[i]);
		memcpy(curImage.getPixels(), rawImage.GetData(), camWidth * camHeight);
		curImage.flagImageChanged();
		curImage.draw(0, 0);

		curImage.threshold(mouseX);
		contourFinder.findContours(curImage, 10, camWidth * camHeight, 8, false);
		contourFinder.draw();

		ofTranslate(camWidth, 0);
	}
	glPopMatrix();

	ofSetColor(255, 255, 255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
}


void testApp::keyPressed(int key){

}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y ){

}


void testApp::mouseDragged(int x, int y, int button){

}


void testApp::mousePressed(int x, int y, int button){

}


void testApp::mouseReleased(int x, int y, int button){

}


void testApp::windowResized(int w, int h){

}

