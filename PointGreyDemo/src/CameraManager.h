#pragma once

#include "FlyCapture2.h"
using namespace FlyCapture2;

#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"

class CameraManager {
public:
	static const int camWidth = 640;
	static const int camHeight = 480;

	static const float minBlobArea = .001;
	static const float maxBlobArea = .2;
	static const int maxBlobs = 8;
	static const int thresholdLevel = 128;

	vector<Camera*> cameras;
	vector<ofxCvGrayscaleImage*> images;
	vector<ofxCvContourFinder*> contourFinders;

	CameraManager() {
	}

	~CameraManager() {
		for(unsigned int i = 0; i < cameras.size(); i++) {
			cameras[i]->StopCapture();
			cameras[i]->Disconnect();
			delete cameras[i];
			delete images[i];
			delete contourFinders[i];
		}
	}

	void catchError(Error error) {
		if(error != PGRERROR_OK)
			error.PrintErrorTrace();
	}

	void printInfo(const CameraInfo& cameraInfo) {
		printf(
			"\n*** CAMERA INFORMATION ***\n"
			"Serial number - %u\n"
			"Camera model - %s\n"
			"Camera vendor - %s\n"
			"Sensor - %s\n"
			"Resolution - %s\n"
			"Firmware version - %s\n"
			"Firmware build time - %s\n\n",
			cameraInfo.serialNumber,
			cameraInfo.modelName,
			cameraInfo.vendorName,
			cameraInfo.sensorInfo,
			cameraInfo.sensorResolution,
			cameraInfo.firmwareVersion,
			cameraInfo.firmwareBuildTime);
	}

	void load(string filename) {
		ofxXmlSettings cameraSettings;
		cameraSettings.loadFile(filename);

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
			printInfo(camInfo);

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

			contourFinders.push_back(new ofxCvContourFinder());
		}
	}

	void update() {
		for(unsigned int i = 0; i < cameras.size(); i++) {
			Camera& camera = *(cameras[i]);
			Image rawImage;
			catchError(camera.RetrieveBuffer(&rawImage));

			memcpy(images[i]->getPixels(), rawImage.GetData(), camWidth * camHeight);
			images[i]->flagImageChanged();
			images[i]->threshold(thresholdLevel);

			int n = camWidth * camHeight;
			contourFinders[i]->findContours(*images[i], minBlobArea * n, maxBlobArea * n, maxBlobs, false);
		}
	}

	void draw() {
		glPushMatrix();
		for(unsigned int i = 0; i < cameras.size(); i++) {
			images[i]->draw(0, 0);
			contourFinders[i]->draw();
			ofTranslate(camWidth, 0);
		}
		glPopMatrix();
	}
};
