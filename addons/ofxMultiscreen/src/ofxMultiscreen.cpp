#include "ofxMultiscreen.h"

bool ofxMultiscreen::master = true; // app is master by default

string ofxMultiscreen::appName = "";
string ofxMultiscreen::appDirectory = "";

string ofxMultiscreen::hostname = "";
MultiCard ofxMultiscreen::card;
int ofxMultiscreen::display = 0;
MultiScreen ofxMultiscreen::localScreen;

vector<MultiComputer> ofxMultiscreen::computers;

ofTrueTypeFont ofxMultiscreen::font;
bool ofxMultiscreen::powersave = true;
bool ofxMultiscreen::debug = true;

void ofxMultiscreen::multiLoad() {
	hostname = getHostname();
	display = getDisplay();

	ofxXmlSettings settings;
	settings.loadFile("settings.xml");

	settings.pushTag("path");
	appName = settings.getValue("appName", "");
	appDirectory = settings.getValue("appDirectory", "");
	settings.popTag();

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
	MultiScreen::setDefaults(settings);
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
				MultiScreen screen(settings, whichScreen);
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

	if(!master) {
		ofHideCursor();
	}
}

void ofxMultiscreen::startScreens() {
	//executeDisplay("xset dpms force on");
	launch("cd " + appDirectory + "; ./" + appName);
}

void ofxMultiscreen::stopScreens() {
	if(powersave) {
		//executeDisplay("xset dpms force off");
	}
	execute("killall " + appName);
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
	ofSetupScreenOrtho(ofGetWidth(), ofGetHeight());

	if(master) {
		ofBackground(128, 128, 128);

		ofxVec2f maxSize = getMaxSize();
		float totalScale = ofGetWidth() / maxSize.x; // assume we normalize on the x axis
		if(totalScale * maxSize.y > ofGetHeight()) // but if this doesn't fit
			totalScale = ofGetHeight() / maxSize.y; // normalize on the y axis instead
		ofxVec2f miniSize = MultiScreen::size * totalScale;

		for(unsigned int i = 0; i < computers.size(); i++) {
			vector<MultiCard>& cards = computers[i].cards;
			for(unsigned int j = 0; j < cards.size(); j++) {
				MultiCard& curCard = cards[j];
				vector<MultiScreen>& screens = curCard.screens;
				for(unsigned int k = 0; k < screens.size(); k++) {
					localScreen = screens[k];
					ofxVec2f position = localScreen.absolutePosition() * totalScale;
					glViewport(position.x, ofGetHeight() - position.y - miniSize.y, miniSize.x, miniSize.y);
					drawScreen();
					ofNoFill();
					ofSetColor(255, 255, 255);
					ofRect(1, 0, ofGetWidthLocal() - 1, ofGetHeightLocal() - 1);
				}
			}
		}
	} else {
		ofxVec2f size = card.getSize();
		vector<MultiScreen>& screens = card.screens;
		for(unsigned int i = 0; i < screens.size(); i++) {
			localScreen = screens[i];
			ofxVec2f placement = card.getPlacement(i);
			glViewport(placement.x, size.y - placement.y - ofGetHeightLocal(), ofGetWidthLocal(), ofGetHeightLocal());
			drawScreen();
		}
	}
}

void ofxMultiscreen::drawScreen() {
	glPushMatrix();
	ofxVec2f position = localScreen.absolutePosition();
	glTranslatef(-position.x, -position.y, 0);
	drawLocal();
	glPopMatrix();

	glPushMatrix();
	drawOverlay();
	if(debug)
		drawDebug();
	glPopMatrix();
}

void ofxMultiscreen::drawDebug() {
	ofSetColor(255, 255, 255);
	ofLine(0, 0, ofGetWidthLocal(), ofGetHeightLocal());
	ofLine(ofGetWidthLocal(), 0, 0, ofGetHeightLocal());
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 40, 60);

	stringstream debugInfo;
	debugInfo << hostname << ":" << display << " @ " << localScreen;
	ofRectangle box = font.getStringBoundingBox(debugInfo.str(), 0, 0);
	ofTranslate((ofGetWidthLocal() - box.width) / 2, (ofGetHeightLocal() - box.height) / 2);
	ofFill();
	ofSetColor(0, 0, 0);
	ofRect(0, 0, box.width, box.height);
	ofSetColor(255, 255, 255);
	font.drawString(debugInfo.str(), 0, box.height);
}

ofxMultiscreen::~ofxMultiscreen() {
	if(master)
		stopScreens();
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
	return MultiScreen::size.x;
}

float ofxMultiscreen::ofGetHeightLocal() {
	return MultiScreen::size.y;
}
