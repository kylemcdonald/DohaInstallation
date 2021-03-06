#include "ofxVirtualCamera.h"

const float fovWidth = 1.0144686707507438;
const float fovHeight = 0.78980943449644714;
const float XtoZ = tanf(fovWidth/2)*2;
const float YtoZ = tanf(fovHeight/2)*2;
const unsigned int camWidth = 640;
const unsigned int camHeight = 480;

ofxVec3f ConvertProjectiveToRealWorld(float x, float y, float z) {
	return ofxVec3f(-(x/camWidth-.5f) * z * XtoZ, // -x for front-mounted cameras
									(y/camHeight-.5f) * z * YtoZ,
									z);
}

ofxVirtualCamera::ofxVirtualCamera() :
newFrame(false),
maxLen(1),
stepSize(1),
nearClipping(-1024),
farClipping(1024),
orthoScale(1),
position(ofxVec3f(0, 0, 0)),
rotation(ofxVec3f(0, 0, 0)) {
}

ofxVirtualCamera::~ofxVirtualCamera() {
	kinect.close();
}

void ofxVirtualCamera::setup() {
	surface.resize(camWidth * camHeight);
	indices.resize(camWidth * camHeight * 3);
	
	kinect.init();
	kinect.open();
	
	fbo.setup(camWidth, camHeight);
	fboColorImage.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
	fbo.attach(fboColorImage.getTextureReference());
	
	fboGrayImage.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
	fboGrayImage.setUseTexture(false);
}

void ofxVirtualCamera::updateSurface() {
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
}

void ofxVirtualCamera::updateMesh() {
	float* z = kinect.getDistancePixels();
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
}

void ofxVirtualCamera::renderCamera() {
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
}

void ofxVirtualCamera::updatePixels() {
	fbo.clearAlpha();
	ofTexture& tex = fboColorImage.getTextureReference();
	tex.bind();
	ofTextureData& data = tex.texData;
	glGetTexImage(data.textureTarget,
								0,
								GL_RGB,
								GL_UNSIGNED_BYTE,
								fboColorImage.getPixels());
	tex.unbind();
	
	// fast color->gray conversion
	unsigned char* grayPixels = fboGrayImage.getPixels();
	unsigned char* colorPixels = fboColorImage.getPixels();
	int n = camWidth * camHeight;
	for(int i = 0; i < n; i++) {
		*grayPixels = *colorPixels;
		grayPixels += 1;
		colorPixels += 3;
	}
}

void ofxVirtualCamera::update() {
	kinect.update();
	if(kinect.isFrameNew()) {
		newFrame = true;
		
		updateSurface();
		updateMesh();
		renderCamera();
		updatePixels();
	}
}

bool ofxVirtualCamera::isFrameNew() {
	bool curNewFrame = newFrame;
	newFrame = false;
	return curNewFrame;
}

unsigned char* ofxVirtualCamera::getPixels() {
	return fboGrayImage.getPixels();
}

void ofxVirtualCamera::draw(float x, float y) {
	fbo.draw(x, y);
}

void ofxVirtualCamera::setMaxLen(float maxLen) {
	this->maxLen = maxLen;
}

void ofxVirtualCamera::setStepSize(int stepSize) {
	this->stepSize = stepSize;
}

void ofxVirtualCamera::setClipping(float nearClipping, float farClipping) {
	this->nearClipping = nearClipping;
	this->farClipping = farClipping;
}

void ofxVirtualCamera::setOrthoScale(float orthoScale) {
	this->orthoScale = orthoScale;
}

void ofxVirtualCamera::setPosition(ofxVec3f position) {
	this->position = position;
}

void ofxVirtualCamera::setRotation(ofxVec3f rotation) {
	this->rotation = rotation;
}

int ofxVirtualCamera::getWidth() const {
	return camWidth;
}

int ofxVirtualCamera::getHeight() const {
	return camHeight;
}