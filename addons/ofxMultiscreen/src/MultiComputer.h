#pragma once

#include "MultiCard.h"

class MultiComputer {
public:
	static bool sendCommands;
	
	string hostname;
	vector<MultiCard> cards;

	MultiComputer(ofxXmlSettings& settings, int which = 0);
	int execute(string command);
	void executeAllDisplays(string command);
	void executeDisplay(string command, int i);
	void launch(string appName);
	
	friend ostream& operator<<(ostream& out, const MultiComputer& computer) {
		out << computer.hostname << "{" << endl;
		for(unsigned int i = 0; i < computer.cards.size(); i++) {
			out << "\t" << computer.cards[i] << endl;
		}
		out << "}";
		return out;
	}
};
