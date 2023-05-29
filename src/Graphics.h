#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include "TrafficObject.h"

using namespace std;

class Graphics
{
public:
    // constructor / desctructor

    // getters / setters
    void setBgFilename(string filename) { _bgFilename = filename; }
    void setTrafficObjects(vector<shared_ptr<TrafficObject>> &trafficObjects) { 
        _trafficObjects = trafficObjects; 
    };

    // typical behaviour methods
    void simulate();

private:
    // typical behaviour methods
    void loadBackgroundImg();
    void drawTrafficObjects();

    // member variables
    vector<shared_ptr<TrafficObject>> _trafficObjects;
    string _bgFilename;
    string _windowName;
    vector<cv::Mat> _images;
};

#endif