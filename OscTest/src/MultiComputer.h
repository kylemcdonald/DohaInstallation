#pragma once

#include "MultiCard.h"

class MultiComputer {
public:
	string hostname;
	vector<MultiCard> cards;

	MultiComputer(ofxXmlSettings& settings, int which = 0) {
		hostname = settings.getAttribute("computer", "hostname", "", which);
	}
	int execute(string command) {
		// run all system calls as background processes
		command = "ssh " + hostname + ".local \"" + command + "\" &";
		if(ofLogLevel() == OF_LOG_VERBOSE)
			cout << ">" << command << endl;
		return system(command.c_str());
	}
	void executeDisplay(string command) {
		for(unsigned int i = 0; i < cards.size(); i++) {
			string card = ofToString((int) i);
			execute("export DISPLAY=:0." + card + "; " + command);
		}
	}
	void launch(string appName) {
		executeDisplay(appName + " 0>~/status 1>&0 2>&0 &");
	}

	friend ostream& operator<<(ostream& out, const MultiComputer& computer) {
		out << computer.hostname << "{" << endl;
		for(unsigned int i = 0; i < computer.cards.size(); i++) {
			out << "\t" << computer.cards[i] << endl;
		}
		out << "}";
		return out;
	}
};
