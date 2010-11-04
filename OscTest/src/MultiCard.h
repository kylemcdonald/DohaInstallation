#pragma once

#include "MultiScreen.h"

class MultiCard {
public:
	vector<MultiScreen> screens;

	MultiCard() {
	}
	ofPoint getSize() const {
		if(screens.empty())
			return ofPoint(1920, 1080); // size for master
		const MultiScreen& last = screens.back();
		ofPoint lastPlacement = getPlacement(screens.size() - 1);
		return ofPoint(lastPlacement.x + last.width, lastPlacement.y + last.height);
	}
	ofPoint getPlacement(int i) const {
		return ofPoint(screens[0].width * ((int) i % 2), screens[0].height * (int) (i / 2));
	}

	friend ostream& operator<<(ostream& out, const MultiCard& card) {
		ofPoint size = card.getSize();
		out << size.x << "x" << size.y << " { ";
		for(unsigned int i = 0; i < card.screens.size(); i++) {
			out << card.screens[i] << " ";
		}
		out << "}";
		return out;
	}
};
