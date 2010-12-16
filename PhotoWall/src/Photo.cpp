#include "Photo.h"

int Photo::photoWidth = 816 / 2;
int Photo::photoHeight = 612 / 2;
float Photo::aspectRatio = (float) Photo::photoWidth / Photo::photoHeight;
float Photo::rotationDamping = .9;
float Photo::maxSpeed = 5;
float Photo::sizeDamping = .9;

float Photo::angleLerp(float from, float to, float t) {
	ofxVec2f fromVec(1, 0);
	ofxVec2f toVec(1, 0);
	fromVec.rotateRad(from);
	toVec.rotateRad(to);
	fromVec.interpolate(toVec, t);
	return atan2f(fromVec.y, fromVec.x);
}

void Photo::setup(const ControlSurface& surface, float x, float y) {
	img = PhotoManager::randomPhoto();
	nw = &(surface.get(floor(x), floor(y)));
	ne = &(surface.get(ceil(x), floor(y)));
	sw = &(surface.get(floor(x), ceil(y)));
	se = &(surface.get(ceil(x), ceil(y)));
	baseColor.set(ofRandom(0, 1), ofRandom(0, 1), ofRandom(0, 1));
}

void Photo::update() {
	float curSize = nw->distance(*se) + ne->distance(*sw);
	curSize /= 4;
	size = ofLerp(curSize, size, sizeDamping);

	ofxVec2f curPosition = *nw + *ne + *sw + *se;
	curPosition /= 4;
	
	if(position.length() == 0) {
		position = curPosition; // start things off on the right foot
	} else {
		ofxVec2f diff = curPosition - position;
		if(diff.length() > maxSpeed) {
			diff.normalize();
			diff *= maxSpeed;
		}
		position += diff;
	}

	ofxVec2f o1 = *sw - *se;
	ofxVec2f o2 = *nw - *ne;
	float curRotation = atan2f(o1.y, o1.x) + atan2f(o2.y, o2.x);
	rotation = angleLerp(curRotation, rotation, rotationDamping);

	brightness = ofMap(size, 200, 400, 1, 0, true);
}

void Photo::draw() const {
	glPushMatrix();

	glTranslatef(position.x, position.y, 0);
	ofRotate(ofRadToDeg(rotation));

	glScalef(size * aspectRatio, size, 1);
	
	/*
	// frame
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-.5, -.5);
	glVertex2f(-.5, .5);
	glVertex2f(.5, .5);
	glVertex2f(.5, -.5);
	glEnd();
	*/
	
	// photo
	//glColor3f(brightness * baseColor.r, brightness * baseColor.g, brightness * baseColor.b);
	
	if(ofxMultiscreen::master) {
		glBegin(GL_QUADS);
		glColor3f(baseColor.r * .2, baseColor.g * .2, baseColor.b * .2);
		glVertex2f(-.5, .5);
		glVertex2f(.5, .5);
		glColor3f(baseColor.r, baseColor.g, baseColor.b);
		glVertex2f(.5, -.5);
		glVertex2f(-.5, -.5);
		glEnd();
	} else {
		img->bind();
		glBegin(GL_QUADS);
		
		glColor3f(.8, .8, .8);
		glTexCoord2f(0, photoHeight);
		glVertex2f(-.5, .5);
		glTexCoord2f(photoWidth, photoHeight);
		glVertex2f(.5, .5);
		
		glColor3f(1, 1, 1);
		glTexCoord2f(photoWidth, 0);
		glVertex2f(.5, -.5);
		glTexCoord2f(0, 0);
		glVertex2f(-.5, -.5);
		glEnd();
		img->unbind();
	}

	glPopMatrix();
}

bool Photo::inside(const ofRectangle& window) const {
	float halfWidth = size * aspectRatio / 2;
	float halfHeight = size / 2;
	float halfDiagonal = sqrtf(halfWidth * halfWidth + halfHeight * halfHeight);
	return position.x + halfDiagonal > window.x && position.y + halfDiagonal > window.y &&
		position.x - halfDiagonal < window.x + window.width && position.y - halfDiagonal < window.y + window.height;
}
