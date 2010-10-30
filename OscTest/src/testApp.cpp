#include "testApp.h"

void testApp::setup(){
    ofxXmlSettings settings, unique;
    settings.loadFile("settings.xml");
    unique.loadFile("unique.xml");

    ofSetLogLevel(OF_LOG_VERBOSE);

    master = unique.getValue("master", 0);
    if(master) {
        cout << "This computer is running as master." << endl;
    } else {
        cout << "This computer is running as a client." << endl;
    }

    settings.pushTag("osc");
    string address = settings.getValue("address", "255.255.255.255");
    int port = settings.getValue("port", 8888);
    settings.popTag();

    if(master) {
        cout << "Connecting to " << address << ":" << port << endl;
        oscSender.setup(address, port);
    } else {
        cout << "Listening on port " << port << endl;
        oscReceiver.setup(port);
    }

    ofEnableAlphaBlending();
}

void testApp::update(){
}

void testApp::draw(){
    ofBackground(0, 0, 0);

    ofPushMatrix();

    if(master) {
        mx = mouseX;
        my = mouseY;
    } else {
        while(oscReceiver.hasWaitingMessages()) {
            ofxOscMessage message;
            oscReceiver.getNextMessage(&message);
            string address = message.getAddress();
            if(address.compare("mouse") == 0) {
                mx = message.getArgAsInt32(0);
                my = message.getArgAsInt32(1);
            }
            scout << "got message at /" << address << endl;
        }
    }

    if(master)
        ofSetColor(255, 0, 0);
    else
        ofSetColor(0, 255, 0);
    ofRect(0, 0, 64, 64);

    ofTranslate(mx, my, 0);
    ofSetColor(255, 255, 255, 1);
    int max = 2048;
    for(int i = 0; i < max; i++) {
        float a = (float) i * TWO_PI / max;
        ofEllipse(sin(a) * 512, cos(a) * 512, 128, 128);
    }

    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    ofPopMatrix();

    ofDrawBitmapString(scout.str(), 10, 20);
}

void testApp::keyPressed(int key){
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y){
    if(master) {
        ofxOscMessage message;
        message.setAddress("mouse");
        message.addIntArg(mx);
        message.addIntArg(my);
        oscSender.sendMessage(message);
        scout << "sent message at /mouse" << endl;
    }
}

void testApp::mouseDragged(int x, int y, int button){
}

void testApp::mousePressed(int x, int y, int button){
}

void testApp::mouseReleased(int x, int y, int button){
}

void testApp::windowResized(int w, int h){
}

