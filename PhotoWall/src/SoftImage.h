#pragma once

#include "ofMain.h"

class SoftImage {
protected:
    string filename;
    bool loadedCpu, loadedGpu;
    ofImage img;
public:
    void setup(string filename) {
        this->filename = filename;
        loadedCpu = false;
        loadedGpu = false;
    }
};
