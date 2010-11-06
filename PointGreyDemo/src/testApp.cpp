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

		cameras.push_back(camera);
	}
}

testApp::~testApp() {
	for(unsigned int i = 0; i < cameras.size(); i++) {
		cameras[i]->StopCapture();
		cameras[i]->Disconnect();
		delete cameras[i];
	}
}


void testApp::update(){

}


void testApp::draw(){
	ofBackground(0, 0, 0);

	int width = 640;
	int height = 480;
/*
	IplImage* cvImageDistorted = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage* cvImageUndistorted = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
*/
	for(unsigned int i = 0; i < cameras.size(); i++) {
		Camera& camera = *(cameras[i]);
		Image image;
		catchError(camera.RetrieveBuffer(&image));

		//int width = image.GetCols();
		//int height = image.GetRows();

		curImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
		memcpy(curImage.getPixels(), image.GetData(), width * height);
		curImage.update();
		curImage.draw(0, 0);
/*
		float radialDistX = -1;
		float radialDistY = -1;
		float tangentDistX = 0;
		float tangentDistY = 0;
		float focalX = mouseX;
		float focalY = mouseY;
		float centerX = width / 2;
		float centerY = height / 2;

		float camIntrinsics[] = {focalX, 0, centerX, 0, focalY, centerY, 0, 0, 1};
    float distortionCoeffs[] = {radialDistX, radialDistY, tangentDistX, tangentDistY};
		memcpy(cvImageDistorted->imageData, curImage.getPixels(), width * height);
    cvUnDistortOnce(cvImageDistorted, cvImageUndistorted, camIntrinsics, distortionCoeffs, 1);

		memcpy(curImage.getPixels(), cvImageUndistorted->imageData, width * height);
		curImage.update();
		curImage.draw(0, height);
		*/

		ofTranslate(width, 0);
	}

/*
	cvReleaseImage(&cvImageDistorted);
	cvReleaseImage(&cvImageUndistorted);
*/
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

