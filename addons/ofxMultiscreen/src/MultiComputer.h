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
		command = "ssh -f " + hostname + ".local \"" + command + "\"";
		if(ofLogLevel() == OF_LOG_VERBOSE)
			cout << ">" << command << endl;
		return system(command.c_str());
	}
	void executeAllDisplays(string command) {
		for(unsigned int i = 0; i < cards.size(); i++) {
			executeDisplay(command, i);
		}
	}
	void executeDisplay(string command, int i) {
		execute("export DISPLAY=:0." + ofToString((int) i) + "; " + command);
	}
	void launch(string appName) {
		for(unsigned int i = 0; i < cards.size(); i++) {
			string card = ofToString((int) i);
			executeDisplay(appName + " </dev/null >~/status." + card + " 2>&1 &", i);
		}
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
