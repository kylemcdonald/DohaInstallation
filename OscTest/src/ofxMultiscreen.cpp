#include "ofxMultiscreen.h"

const string ofxMultiscreen::appName = "OscTest";
const string ofxMultiscreen::appDirectory = "~/Desktop/openFrameworks/apps/DohaInstallation/OscTest/bin";

bool ofxMultiscreen::master = true; // app is master by default
int ofxMultiscreen::display = 0;
string ofxMultiscreen::hostname;
MultiCard ofxMultiscreen::card;
vector<MultiComputer> ofxMultiscreen::computers;
bool ofxMultiscreen::powersave = true;
ofxFbo ofxMultiscreen::fbo;
vector<ofTexture*> ofxMultiscreen::renderBuffers;
MultiScreen ofxMultiscreen::localScreen;

void ofxMultiscreen::multiLoad() {
	hostname = getHostname();
	display = getDisplay();

	ofxXmlSettings settings;
	settings.loadFile("settings.xml");
	settings.pushTag("options");
	powersave = settings.getValue("powersave", 0);
	settings.popTag();

	loadScreens(settings);

	if(ofLogLevel() == OF_LOG_VERBOSE) {
		if(master) {
			cout << "This computer is running as master." << endl;
		} else {
			cout << "This computer is running as a client." << endl;
		}
		cout << "Running on computer " << hostname << " on display " << display << endl;
	}
}

void ofxMultiscreen::loadScreens(ofxXmlSettings& settings) {
	settings.pushTag("defaults");
	MultiScreen defaultScreen(settings, MultiScreen());
	settings.popTag();

	settings.pushTag("computers");
	int nComputers = settings.getNumTags("computer");
	for(int whichComputer = 0; whichComputer < nComputers; whichComputer++) {
		MultiComputer curComputer(settings, whichComputer);
		settings.pushTag("computer", whichComputer);
		int nWindows = settings.getNumTags("card");
		for(int whichWindow = 0; whichWindow < nWindows; whichWindow++) {
			MultiCard curCard;
			settings.pushTag("card", whichWindow);
			int nScreens = settings.getNumTags("screen");
			for(int whichScreen = 0; whichScreen < nScreens; whichScreen++) {
				MultiScreen screen(settings, defaultScreen, whichScreen);
				curCard.screens.push_back(screen);
			}
			settings.popTag();
			curComputer.cards.push_back(curCard);

			if(whichWindow == display && curComputer.hostname.compare(hostname) == 0) {
				master = false;
				card = curCard;
			}
		}
		settings.popTag();
		computers.push_back(curComputer);
	}
	settings.popTag();

	if(ofLogLevel() == OF_LOG_VERBOSE) {
		for(unsigned int i = 0; i < computers.size(); i++) {
			cout << computers[i] << endl;
		}
	}
}

void ofxMultiscreen::multiSetup() {
	if(!master) {
		// allocate FBOs for rendering into
		vector<MultiScreen>& screens = card.screens;
		fbo.setup(screens[0].width, screens[0].height);
		for(unsigned int i = 0; i < screens.size(); i++) {
			MultiScreen& curScreen = screens[i];
			ofTexture* tex = new ofTexture();
			tex->allocate(curScreen.width, curScreen.height, GL_RGBA); // or just GL_RGB
			renderBuffers.push_back(tex);
		}

		ofHideCursor();
	}
}

void ofxMultiscreen::startScreens() {
	executeDisplay("xset dpms force on");
	launch("cd " + appDirectory + "; ./" + appName);
}

void ofxMultiscreen::stopScreens() {
	if(powersave)
		executeDisplay("xset dpms force off");
	execute("killall -9 " + appName);
}

void ofxMultiscreen::execute(string command) {
	for(unsigned int i = 0; i < computers.size(); i++) {
		computers[i].execute(command);
	}
}

void ofxMultiscreen::executeDisplay(string command) {
	for(unsigned int i = 0; i < computers.size(); i++) {
		computers[i].executeDisplay(command);
	}
}

void ofxMultiscreen::launch(string appName) {
	for(unsigned int i = 0; i < computers.size(); i++) {
		computers[i].launch(appName);
	}
}

void ofxMultiscreen::draw() {
	ofBackground(0, 0, 0);
	ofSetupScreenOrtho(ofGetWidth(), ofGetHeight());

	if(master) {
		glPushMatrix();
		drawLocal();
		glPopMatrix();

		glPushMatrix();
		drawOverlay();
		glPopMatrix();
	} else {
		vector<MultiScreen>& screens = card.screens;
		for(unsigned int i = 0; i < screens.size(); i++) {
			localScreen = screens[i];

			fbo.attach(*renderBuffers[i]);
			fbo.begin();
			fbo.setBackground(0, 0, 0);

			glPushMatrix();
			glTranslatef(-localScreen.absoluteX(), -localScreen.absoluteY(), 0);
			drawLocal();
			glPopMatrix();

			glPushMatrix();
			drawOverlay();
			glPopMatrix();

			fbo.end();

			ofSetupScreenOrtho(ofGetWidth(), ofGetHeight());
			glPushMatrix();
			glColor4f(1, 1, 1, 1);
			ofPoint placement = card.getPlacement(i);
			fbo.draw(placement.x, placement.y);
			glPopMatrix();
		}
	}
}

ofxMultiscreen::~ofxMultiscreen() {
	if(master)
		stopScreens();
	for(unsigned int i = 0; i < renderBuffers.size(); i++)
		delete renderBuffers[i];
}

#define MAX_HOSTNAME_LENGTH 256
string ofxMultiscreen::getHostname() {
	char hostname[MAX_HOSTNAME_LENGTH];
	gethostname(hostname, MAX_HOSTNAME_LENGTH);
	return hostname;
}

int ofxMultiscreen::getDisplay() {
	string display(getenv("DISPLAY"));
	vector<string> parts = ofSplitString(display, ".");
	return ofToInt(parts[1]);
}

float ofxMultiscreen::ofGetWidthLocal() {
	return localScreen.width;
}

float ofxMultiscreen::ofGetHeightLocal() {
	return localScreen.height;
}
