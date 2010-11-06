#pragma once

#include "MultiScreen.h"

class MultiCard {
public:
	vector<MultiScreen> screens;

	MultiCard() {
	}
	ofxVec2f getSize() const {
		if(screens.empty())
			return MultiScreen::size; // for master
		return getPlacement(screens.size() - 1) + screens.back().size;
	}
	ofxVec2f getPlacement(int i) const {
		return ofxVec2f(screens[0].size.x * ((int) i % 2), screens[0].size.y * (int) (i / 2));
	}

	friend ostream& operator<<(ostream& out, const MultiCard& card) {
		ofxVec2f size = card.getSize();
		out << size.x << "x" << size.y << " { ";
		for(unsigned int i = 0; i < card.screens.size(); i++) {
			out << card.screens[i] << " ";
		}
		out << "}";
		return out;
	}
};
