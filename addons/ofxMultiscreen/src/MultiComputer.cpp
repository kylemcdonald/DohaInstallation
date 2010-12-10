#include "MultiComputer.h"

bool MultiComputer::sendCommands = false;

MultiComputer::MultiComputer(ofxXmlSettings& settings, int which) {
	hostname = settings.getAttribute("computer", "hostname", "", which);
}

int MultiComputer::execute(string command) {
	command = "ssh -f " + hostname + ".local \"" + command + "\"";
	if(sendCommands) {
	    ofLog(OF_LOG_VERBOSE, ">" + command);
		return system(command.c_str());
	} else {
	    ofLog(OF_LOG_VERBOSE, "(>" + command + ")");
		return 0;
	}
}

void MultiComputer::executeAllDisplays(string command) {
	for(unsigned int i = 0; i < cards.size(); i++) {
		executeDisplay(command, i);
	}
}

void MultiComputer::executeDisplay(string command, int i) {
	execute("export DISPLAY=:0." + ofToString((int) i) + "; " + command);
}

void MultiComputer::launch(string appName) {
	for(unsigned int i = 0; i < cards.size(); i++) {
		string card = ofToString((int) i);
		executeDisplay(appName + " </dev/null >~/status." + card + " 2>&1 &", i);
	}
}
