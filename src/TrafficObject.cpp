#include <algorithm>
#include <iostream>
#include <chrono>
#include "TrafficObject.h"

using namespace std;

// init static variable
int TrafficObject::_idCnt = 0;

mutex TrafficObject::_mtx;

void TrafficObject::setPosition(double x, double y) {
    _posX = x;
    _posY = y;
}

void TrafficObject::getPosition(double &x, double &y) {
    x = _posX;
    y = _posY;
}
