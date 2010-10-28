#include "testApp.h"

void testApp::setup(){
    ofEnableAlphaBlending();
}

void testApp::update(){
}

void testApp::draw(){
    ofBackground(0, 0, 0);

    ofTranslate(mouseX, mouseY, 0);
    ofSetColor(255, 255, 255, 1);
    int max = 2048;
    for(int i = 0; i < max; i++) {
        float a = (float) i * TWO_PI / max;
        ofEllipse(sin(a) * 512, cos(a) * 512, 128, 128);
    }

    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}

void testApp::keyPressed(int key){
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y){
}

void testApp::mouseDragged(int x, int y, int button){
}

void testApp::mousePressed(int x, int y, int button){
}

void testApp::mouseReleased(int x, int y, int button){
}

void testApp::windowResized(int w, int h){
}

