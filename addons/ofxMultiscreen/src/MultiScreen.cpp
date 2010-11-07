#include "MultiScreen.h"

ofxVec2f MultiScreen::size;
int MultiScreen::bevel;

void MultiScreen::setDefaults(ofxXmlSettings& settings, int which) {
	size.x = settings.getAttribute("screen", "width", 0, which);
	size.y = settings.getAttribute("screen", "height", 0, which);
	bevel = settings.getAttribute("screen", "bevel", 0, which);
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

