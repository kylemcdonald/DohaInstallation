#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	panel.setup("Control Panel", 640 - 140, 240, 280, 600);
	panel.addPanel("Contours");
	panel.addSlider("alpha", "alpha", .1, 0, 1);
	panel.addSlider("blurAmount", "blurAmount", 4, 0, 16, true);
	panel.addSlider("threshLevel", "threshLevel", 64, 0, 255, true);
	panel.addSlider("minArea", "minArea", 200, 0, 100 * 100, true);
	panel.addSlider("maxArea", "maxArea", 10000, 32 * 32, 240 * 240, true);
	panel.addSlider("nConsidered", "nConsidered", 8, 1, 16, true);

	panel.addPanel("Camera");
	panel.addSlider("maxLen", "maxLen", 8, 0, 20);
	panel.addSlider("stepSize", "stepSize", 1, 1, 10, true);
	panel.addSlider("nearClipping", "nearClipping", 320, 0, 1024);
	panel.addSlider("farClipping", "farClipping", 350, 0, 1024);
	panel.addSlider("orthoScale", "orthoScale", .65, 0, 2);
	panel.addSlider("camx", "camx", 0, -1024, 1024);
	panel.addSlider("camy", "camy", -148, -1024, 1024);
	panel.addSlider("camz", "camz", 0, -1024, 1024);
	panel.addSlider("camrx", "camrx", 20.2, -180, 180);
	panel.addSlider("camry", "camry", 0, -180, 180);
	panel.addSlider("camrz", "camrz", 0, -180, 180);

	cam.setup();

	blur.allocate(640, 480);
	thresh.allocate(640, 480);

	setupOsc();
}

void testApp::setupOsc() {
	ofxXmlSettings settings;
	if(!settings.loadFile("osc.xml")) {
		ofLog(OF_LOG_ERROR, "testApp::setupOsc(): couldn't load osc.xml");
	}
	string address = settings.getValue("address", "255.255.255.255");
	int port = settings.getValue("port", 8888);

	cout << "Broadcasting to " << address << ":" << port << endl;
	oscSender.setup(address, port);
}

IplImage* toCv(unsigned char* pixels, int width, int height, int type) {
	int channels = type == OF_IMAGE_COLOR ? 3 : 1;
	IplImage* ipl = cvCreateImageHeader(cvSize(width, height), IPL_DEPTH_8U, channels);
	cvSetData(ipl, pixels, channels * width);
	return ipl;
}

void testApp::updateOsc() {
	ofxOscMessage message;
	message.setAddress("mouse");

	vector<ofxCvBlob>& blobs = finder.blobs;
	for(int i = 0; i < blobs.size(); i++) {
		ofxVec2f curPoint(blobs[i].centroid);
		// squeeze because the floor has a different aspect ratio than the camera
		curPoint.x = ofMap(curPoint.x, 0, 640, .15, .85);
		curPoint.y = ofMap(curPoint.y, 0, 480, 0, 1);
 
		//project forward
		message.addFloatArg(curPoint.x);
		message.addFloatArg(.8);
 
		// project upward
		message.addFloatArg(curPoint.x);
		message.addFloatArg(ofMap(curPoint.y, 0, 1, 0, .6));
	}
 
	oscSender.sendMessage(message);
}

vector<ofPoint> ofGetSmoothed(const vector<ofPoint>& polyline, int smoothingSize, float smoothingShape = 0) {
	vector<ofPoint> result = polyline;
 
// precompute weights and normalization
	vector<float> weights;
	float weightSum = 0;
	weights.push_back(1); // center weight
// side weights
	for(int i = 1; i <= smoothingSize; i++) {
		float curWeight = ofMap(i, 0, smoothingSize, 1, smoothingShape);
		weights.push_back(curWeight);
		weightSum += curWeight;
	}
	float weightNormalization = 1 / (1 + 2 * weightSum);
 
// use weights to make weighted averages of neighbors
	int n = polyline.size();
	for(int i = 0; i < n; i++) {
		for(int j = 1; j <= smoothingSize; j++) {
			int leftPosition = (n + i - j) % n;
			int rightPosition = (i + j) % n;
			const ofPoint& left = polyline[leftPosition];
			const ofPoint& right = polyline[rightPosition];
			result[i] += (left + right) * weights[j];
		}
		result[i] *= weightNormalization;
	}
 
	return result;
}

//--------------------------------------------------------------
void testApp::update() {
	cam.setMaxLen(panel.getValueF("maxLen"));
	cam.setStepSize(panel.getValueF("stepSize"));
	cam.setClipping(panel.getValueF("nearClipping"), panel.getValueF("farClipping"));
	cam.setOrthoScale(panel.getValueF("orthoScale"));
	cam.setPosition(ofxVec3f(panel.getValueF("camx"), panel.getValueF("camy"), panel.getValueF("camz")));
	cam.setRotation(ofxVec3f(panel.getValueF("camrx"), panel.getValueF("camry"), panel.getValueF("camrz")));
 
	int blurAmount = panel.getValueI("blurAmount");
	int threshLevel = panel.getValueI("threshLevel");
	int minArea = panel.getValueI("minArea");
	int maxArea = panel.getValueI("maxArea");
	int nConsidered = panel.getValueI("nConsidered");
 
	cam.update();
	if(cam.isFrameNew()) {
		float alpha = panel.getValueF("alpha");
		float beta = 1 - alpha;
		IplImage* camIpl = toCv(cam.getPixels(), cam.getWidth(), cam.getHeight(), OF_IMAGE_GRAYSCALE);
		cvAddWeighted(camIpl, alpha, blur.getCvImage(), beta, 0, blur.getCvImage());
		blur.flagImageChanged();
		blur.blur(blurAmount * 2 + 1);
 
		thresh = blur;
		thresh.threshold(threshLevel);
		finder.findContours(thresh, minArea, maxArea, nConsidered, false, false);
 
		// make the average the centroid
		// should be more stable than the moments
		vector<ofxCvBlob>& blobs = finder.blobs;
		for(int i = 0; i < blobs.size(); i++) {
			ofxCvBlob& cur = blobs[i];
			vector<ofPoint>& pts = cur.pts;
			
			pts = ofGetSmoothed(pts, 8);
			
			ofPoint& centroid = cur.centroid;
			centroid.set(0, 0);
			for(int j = 0; j < pts.size(); j++) {
				centroid += pts[j];
			}
			centroid /= pts.size();
		}
 
		updateOsc();
	}
}
 
//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(0, 0, 0);
	ofSetColor(255, 255, 255);
	cam.draw(0, 0);
 
	blur.draw(640, 0);
	ofEnableAlphaBlending();
	ofSetColor(255, 0, 0, 128);
	thresh.draw(640, 0);
	ofDisableAlphaBlending();
 
	finder.draw(0, 0);
 
	vector<ofxCvBlob>& blobs = finder.blobs;
	for(int i = 0; i < blobs.size(); i++) {
		ofPoint& cur = blobs[i].centroid;
		ofCircle(cur.x, cur.y, 5);
	}
 
	ofSetColor(255, 0, 0);
	ofLine(320, 0, 320, 480);
	ofLine(0, 240, 640, 240);
	ofNoFill();
	ofRect(0, 0, 640, 480);
}
 
void testApp::keyPressed(int key) {
}
 