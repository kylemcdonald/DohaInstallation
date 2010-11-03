#pragma once

#include "MultiWindow.h"

class MultiComputer {
public:
	string hostname;
	vector<MultiWindow> windows;

	MultiComputer(ofxXmlSettings& settings, int which = 0) {
		hostname = settings.getAttribute("computer", "hostname", "", which);
	}
	int execute(string command) {
		command = "ssh " + hostname + ".local \"" + command + "\"";
		cout << "system(" << command << ")" << endl;
		return system(command.c_str());
	}
	void launch(string appName) {
		for(unsigned int i = 0; i < windows.size(); i++) {
			string window = ofToString((int) i);
			execute("export DISPLAY=:0." + window + "; " + appName + " " + window + " 0>/dev/null 1>&0 2>&0 &");
		}
	}

	friend ostream& operator<<(ostream& out, const MultiComputer& computer) {
		out << computer.hostname << "{" << endl;
		for(unsigned int i = 0; i < computer.windows.size(); i++) {
			out << "\t" << computer.windows[i] << endl;
		}
		out << "}";
		return out;
	}
};
