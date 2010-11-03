#pragma once

#include "MultiScreen.h"

class MultiCard {
public:
	vector<MultiScreen> screens;

	MultiCard() {
	}
	float getWidth() const {
		float width = 0;
		for(unsigned int i = 0; i < screens.size(); i++)
			width += screens[i].width;
		return width;
	}
	float getHeight() const {
		float height = 0;
		for(unsigned int i = 0; i < screens.size(); i++)
			height += screens[i].height;
		return height;
	}

	friend ostream& operator<<(ostream& out, const MultiCard& card) {
		out << card.getWidth() << "x" << card.getHeight() << " { ";
		for(unsigned int i = 0; i < card.screens.size(); i++) {
			out << card.screens[i] << " ";
		}
		out << "}";
		return out;
	}
};
