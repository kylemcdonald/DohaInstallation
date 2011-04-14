#include "VirtualCamera.h"

const float fovWidth = 1.0144686707507438;
const float fovHeight = 0.78980943449644714;
const float XtoZ = tanf(fovWidth/2)*2;
const float YtoZ = tanf(fovHeight/2)*2;
const unsigned int camWidth = 640;
const unsigned int camHeight = 480;

ofxVec3f ConvertProjectiveToRealWorld(float x, float y, float z) {
	return ofxVec3f((x/camWidth-.5f) * z * XtoZ,
									(y/camHeight-.5f) * z * YtoZ,
									z);
}

VirtualCamera::VirtualCamera() :
newFrame(false),
maxLen(1),
stepSize(1),
nearClipping(-1024),
farClipping(1024),
orthoScale(1),
position(ofxVec3f(0, 0, 0)),
rotation(ofxVec3f(0, 0, 0)) {
}

VirtualCamera::~VirtualCamera() {
	kinect.close();
}

void VirtualCamera::setup() {
	surface.resize(camWidth * camHeight);
	indices.resize(camWidth * camHeight * 3);
	
	kinect.init();
	kinect.open();
	
	fbo.setup(camWidth, camHeight);
	fboPixels.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
	fboPixels.setUseTexture(false);
}


void VirtualCamera::update() {
	kinect.update();
	if(kinect.isFrameNew()) {
		newFrame = true;
		
		float* z = kinect.getDistancePixels();
		
		int i = 0;
		for(int y = 0; y < camHeight; y++) {
			for(int x = 0; x < camWidth; x++) {
				if(z[i] != 0) {
					surface[i] = ConvertProjectiveToRealWorld(x, y, -z[i]);
				}
				i++;
			}
		}
		
		indices.clear();
		for(int y = 0; y < camHeight - stepSize; y += stepSize) {
			for(int x = 0; x < camWidth - stepSize; x += stepSize) {
				int i = y * camWidth + x;
				unsigned int nwi = i;
				unsigned int nei = nwi + stepSize;
				unsigned int swi = nwi + (stepSize * camWidth);
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
		
		fbo.begin();
		fbo.setBackground(0, 0, 0, 255);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_FOG);
		
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
		glOrtho(-320 * orthoScale, 320 * orthoScale, -240 * orthoScale, 240 * orthoScale, -1024, 1024);
		glMatrixMode(GL_MODELVIEW);
		
		glLoadIdentity();
		glScalef(1, -1, 1);
		
		ofTranslate(position.x, position.y, position.z);
		ofRotateX(rotation.x);
		ofRotateY(rotation.y);
		ofRotateZ(rotation.z);
		
		ofSetColor(255, 255, 255);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(ofxVec3f), &(surface[0].x));
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
		glDisableClientState(GL_VERTEX_ARRAY);
		
		ofPopMatrix();
		
		glDisable(GL_FOG);
		glDisable(GL_DEPTH_TEST);
		
		fbo.end();
		
		fbo.clearAlpha();
		fbo.getPixels(fboPixels.getPixels());
	}
}

bool VirtualCamera::isFrameNew() {
	bool curNewFrame = newFrame;
	newFrame = false;
	return curNewFrame;
}

unsigned char* VirtualCamera::getPixels() {
	return fboPixels.getPixels();
}

void VirtualCamera::draw(float x, float y) {
	fbo.draw(x, y);
}

void VirtualCamera::setMaxLen(float maxLen) {
	this->maxLen = maxLen;
}

void VirtualCamera::setStepSize(int stepSize) {
	this->stepSize = stepSize;
}

void VirtualCamera::setClipping(float nearClipping, float farClipping) {
	this->nearClipping = nearClipping;
	this->farClipping = farClipping;
}

void VirtualCamera::setOrthoScale(float orthoScale) {
	this->orthoScale = orthoScale;
}

void VirtualCamera::setPosition(ofxVec3f position) {
	this->position = position;
}

void VirtualCamera::setRotation(ofxVec3f rotation) {
	this->rotation = rotation;
}
