#pragma once

#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"

class MultiScreen {
public:
	static ofxVec2f size;
	static void setDefaults(ofxXmlSettings& settings, int which = 0);

	ofxVec2f position;
	MultiScreen();
	MultiScreen(ofxXmlSettings& settings, int which = 0);
	ofxVec2f absolutePosition() const;
	ofxVec2f getMaxSize() const;

	friend ostream& operator<<(ostream& out, const MultiScreen& screen) {
		out << screen.position.x << "/" << screen.position.y;
		return out;
	}
};
