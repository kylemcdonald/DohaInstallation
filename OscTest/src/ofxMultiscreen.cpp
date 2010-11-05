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
ofTrueTypeFont ofxMultiscreen::font;
bool ofxMultiscreen::debug = true;

void ofxMultiscreen::multiLoad() {
	hostname = getHostname();
	display = getDisplay();

	ofxXmlSettings settings;
	settings.loadFile("settings.xml");
	settings.pushTag("options");
	powersave = settings.getValue("powersave", 0);
	debug = settings.getValue("debug", 0);
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
	font.loadFont("liberation.ttf", 80);

	// allocate textures for rendering into
	if(master) {
		MultiScreen& screen = computers[0].cards[0].screens[0];
		fbo.setup(screen.width, screen.height);
		for(unsigned int i = 0; i < computers.size(); i++) {
			vector<MultiCard>& cards = computers[i].cards;
			for(unsigned int j = 0; j < cards.size(); j++) {
				addTexturesForScreens(cards[j].screens);
			}
		}
	} else {
		MultiScreen& screen = card.screens[0];
		fbo.setup(screen.width, screen.height);
		addTexturesForScreens(card.screens);
		ofHideCursor();
	}
}

void ofxMultiscreen::addTexturesForScreens(vector<MultiScreen>& screens) {
	for(unsigned int i = 0; i < screens.size(); i++) {
		localScreen = screens[i];
		ofTexture* tex = new ofTexture();
		tex->allocate(localScreen.width, localScreen.height, GL_RGBA);
		renderBuffers.push_back(tex);
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

ofPoint ofxMultiscreen::getMaxSize() {
	ofPoint maxSize;
	for(unsigned int i = 0; i < computers.size(); i++) {
		vector<MultiCard>& cards = computers[i].cards;
		for(unsigned int j = 0; j < cards.size(); j++) {
			vector<MultiScreen>& screens = cards[j].screens;
			for(unsigned int k = 0; k < screens.size(); k++) {
				ofPoint curSize = screens[k].getMaxSize();
				if(curSize.x > maxSize.x)
					maxSize.x = curSize.x;
				if(curSize.y > maxSize.y)
					maxSize.y = curSize.y;
			}
		}
	}
	return maxSize;
}

void ofxMultiscreen::draw() {
	ofPoint size = card.getSize();
	ofSetupScreenOrtho(size.x, size.y);

	if(master) {
		ofBackground(0, 0, 0);

		ofPoint maxSize = getMaxSize();
		float totalScale = size.x / maxSize.x; // assume we normalize on the x axis
		if(totalScale * maxSize.y > size.y) // but if this doesn't fit
			totalScale = size.y / maxSize.y; // normalize on the y axis instead

		int m = 0;
		for(unsigned int i = 0; i < computers.size(); i++) {
			vector<MultiCard>& cards = computers[i].cards;
			for(unsigned int j = 0; j < cards.size(); j++) {
				MultiCard& curCard = cards[j];
				vector<MultiScreen>& screens = curCard.screens;
				for(unsigned int k = 0; k < screens.size(); k++) {
					localScreen = screens[k];

					fbo.attach(*renderBuffers[m++]);
					fbo.begin();
					fbo.setBackground(0, 0, 0);
					drawScreen();
					fbo.end();

					glPushMatrix();
					ofSetupScreenOrtho(size.x, size.y);
					glScalef(totalScale, totalScale, totalScale);
					glColor4f(1, 1, 1, 1);
					glTranslatef(localScreen.absoluteX(), localScreen.absoluteY(), 0);
					fbo.draw(0, 0, localScreen.width, localScreen.height);
					ofNoFill();
					ofRect(0, 0, localScreen.width, localScreen.height);
					glPopMatrix();
				}
			}
		}
	} else {
		vector<MultiScreen>& screens = card.screens;
		for(unsigned int i = 0; i < screens.size(); i++) {
			localScreen = screens[i];

			fbo.attach(*renderBuffers[i]);
			fbo.begin();
			fbo.setBackground(0, 0, 0);
			drawScreen();
			fbo.end();

			glPushMatrix();
			ofPoint placement = card.getPlacement(i);
			ofSetupScreenOrtho(ofGetWidth(), ofGetHeight());
			glColor4f(1, 1, 1, 1);
			fbo.draw(placement.x, placement.y);
			glPopMatrix();
		}
	}
}

void ofxMultiscreen::drawScreen() {
	glPushMatrix();
	glTranslatef(-localScreen.absoluteX(), -localScreen.absoluteY(), 0);
	drawLocal();
	glPopMatrix();

	glPushMatrix();
	stringstream debugInfo;
	drawOverlay();
	if(debug) {
		debugInfo << hostname << ":" << display << " @ " << localScreen.x << "/" << localScreen.y;
		font.drawString(debugInfo.str(), 0, ofGetHeightLocal() / 2);
	}
	glPopMatrix();
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
