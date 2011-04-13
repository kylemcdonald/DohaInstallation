#include "testApp.h"

const float FovH = 1.0144686707507438;
const float FovV = 0.78980943449644714;
const float XtoZ = tanf(FovH/2)*2;
const float YtoZ = tanf(FovV/2)*2;
const unsigned int Xres = 640;
const unsigned int Yres = 480;

ofxVec3f ConvertProjectiveToRealWorld(float x, float y, float z) {
	return ofxVec3f((x/Xres-.5f) * z * XtoZ,
								 (y/Yres-.5f) * z * YtoZ,
								 z);
}

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	
	panel.setup("Control Panel", 5, 5, 280, 600);
	panel.addPanel("Camera");
	panel.addSlider("maxLen", "maxLen", 8, 0, 20);
	panel.addSlider("stepSize", "stepSize", 1, 1, 10, true);
	panel.addSlider("nearClipping", "nearClipping", 380, 0, 1024);
	panel.addSlider("farClipping", "farClipping", 600, 0, 1024);
	panel.addSlider("orthoScale", "orthoScale", .66, 0, 2);
	panel.addSlider("camx", "camx", 0, -1024, 1024);
	panel.addSlider("camy", "camy", -240, -1024, 1024);
	panel.addSlider("camz", "camz", -500, -1024, 1024);
	panel.addSlider("camrx", "camrx", 90, -180, 180);
	panel.addSlider("camry", "camry", 0, -180, 180);
	panel.addSlider("camrz", "camrz", 0, -180, 180);
	
	surface.resize(640 * 480);
	indices.resize(640 * 480 * 3);
	
	kinect.init();
	kinect.open();
}

//--------------------------------------------------------------
void testApp::update() {
	kinect.update();
	if(kinect.isFrameNew()) {
		float* z = kinect.getDistancePixels();
		
		int i = 0;
		for(int y = 0; y < Yres; y++) {
			for(int x = 0; x < Xres; x++) {
				if(z[i] != 0) {
					surface[i] = ConvertProjectiveToRealWorld(x, y, -z[i]);
				}
				i++;
			}
		}
		
		indices.clear();
		float maxLen = panel.getValueF("maxLen");
		int stepSize = panel.getValueI("stepSize");
		for(int y = 0; y < Yres - stepSize; y += stepSize) {
			for(int x = 0; x < Xres - stepSize; x += stepSize) {
				int i = y * Xres + x;
				unsigned int nwi = i;
				unsigned int nei = nwi + stepSize;
				unsigned int swi = nwi + (stepSize * Xres);
				unsigned int sei = swi + stepSize;
				float nw = z[nwi];
				float ne = z[nei];
				float sw = z[swi];
				float se = z[sei];
				
				if(nw != 0 && ne != 0 && sw != 0 &&
					 abs(nw - ne) < maxLen && abs(nw - sw) < maxLen) {
					indices.push_back(nwi);
					indices.push_back(nei);
					indices.push_back(swi);
				}
				
				if(ne != 0 && se != 0 && sw != 0 &&
					 abs(sw - se) < maxLen && abs(ne - se) < maxLen) {
					indices.push_back(nei);
					indices.push_back(sei);
					indices.push_back(swi);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(0, 0, 0);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FOG);
	
	float nearClipping = panel.getValueF("nearClipping");
	float farClipping = panel.getValueF("farClipping");
	glClearColor(0, 0, 0, 1);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	GLfloat fogColor[4]= {0, 0, 0, 1};
	glFogfv(GL_FOG_COLOR, fogColor);
	glHint(GL_FOG_HINT, GL_FASTEST);
	glFogf(GL_FOG_START, nearClipping);
	glFogf(GL_FOG_END, farClipping);
	
	ofPushMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	float orthoScale = panel.getValueF("orthoScale");
	glOrtho(-320 * orthoScale, 320 * orthoScale, -240 * orthoScale, 240 * orthoScale, -1024, 1024);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	
	ofTranslate(panel.getValueF("camx"), panel.getValueF("camy"), panel.getValueF("camz"));
	ofRotateX(panel.getValueF("camrx"));
	ofRotateY(panel.getValueF("camry"));
	ofRotateZ(panel.getValueF("camrz"));
	
	ofSetColor(255, 255, 255);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofxVec3f), &(surface[0].x));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	ofPopMatrix();
	
	glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
	
	ofSetupScreen();
}

//--------------------------------------------------------------
void testApp::exit() {
	kinect.close();
}

void testApp::keyPressed(int key) {
}