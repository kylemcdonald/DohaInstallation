#pragma once

class MultiScreen {
public:
	float x, y;
	float width, height;
	string mode;

	MultiScreen() :
		x(0),
		y(0),
		width(0),
		height(0),
		mode("relative") {
	}
	MultiScreen(ofxXmlSettings& settings, MultiScreen defaultScreen, int which = 0) {
		x = settings.getAttribute("screen", "x", defaultScreen.x, which);
		y = settings.getAttribute("screen", "y", defaultScreen.y, which);
		width = settings.getAttribute("screen", "width", defaultScreen.width, which);
		height = settings.getAttribute("screen", "height", defaultScreen.height, which);
		mode = settings.getAttribute("screen", "mode", defaultScreen.mode, which);
	}
	bool relative() const {
		return mode.compare("relative") == 0;
	}
	float absoluteX() const {
		return (relative() ? width : 1) * x;
	}
	float absoluteY() const {
		return (relative() ? height : 1) * y;
	}

	friend ostream& operator<<(ostream& out, const MultiScreen& screen) {
		out << screen.width << "x" << screen.height << "@" << screen.x << "/" << screen.y << ":" << screen.mode;
		return out;
	}
};
