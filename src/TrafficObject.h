#ifndef TRAFFICOBJECT_H
#define TRAFFICOBJECT_H

#include <vector>
#include <thread>
#include <mutex>

using namespace std;

enum ObjectType
{
    noObject,
    objectVehicle,
    objectIntersection,
    objectStreet,
    objectTrafficLight,
};

class TrafficObject
{
protected:
    ObjectType _type;                 // identifies the class type
    int _id;                          // every traffic object has its own unique id
    double _posX, _posY;              // vehicle position in pixels
    vector<thread> threads; // holds all threads that have been launched within this object
    static mutex _mtx;           // mutex shared by all traffic objects for protecting cout 

private:
    static int _idCnt; // global variable for counting object ids

public:
    // constructor / desctructor
    TrafficObject() {
        _type = ObjectType::noObject;
        _id = _idCnt++;
    }

    ~TrafficObject() {
        // set up thread barrier before this object is destroyed
        for_each(threads.begin(), threads.end(), [](thread &t) {
            t.join();
        });
    }

    // getter and setter
    int getID() { 
        return _id; 
    }

    void setPosition(double x, double y);

    void getPosition(double &x, double &y);

    ObjectType getType() { 
        return _type; 
    }

    // typical behaviour methods
    virtual void simulate(){};
};

#endif