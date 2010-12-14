#include "MultiScreen.h"

ofxVec2f MultiScreen::size;
ofxVec2f MultiScreen::bevel;

void MultiScreen::setDefaults(ofxXmlSettings& settings, int which) {
	size.x = settings.getAttribute("screen", "width", 0, which);
	size.y = settings.getAttribute("screen", "height", 0, which);
	bevel.x = settings.getAttribute("screen", "bevelWidth", 0, which);
	bevel.y = settings.getAttribute("screen", "bevelHeight", 0, which);
}

MultiScreen::MultiScreen() {
}

MultiScreen::MultiScreen(ofxXmlSettings& settings, int which) {
	position.x = settings.getAttribute("screen", "x", 0, which);
	position.y = settings.getAttribute("screen", "y", 0, which);
}

ofxVec2f MultiScreen::absolutePosition() const {
	return position * (size + bevel);
}

ofxVec2f MultiScreen::getMaxSize() const {
	return absolutePosition() + size;
}

