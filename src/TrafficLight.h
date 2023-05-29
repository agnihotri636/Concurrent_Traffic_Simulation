#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

using namespace std;

// forward declarations to avoid include cycle
class Vehicle;
class Intersection;

template <class T>
class MessageQueue
{
public:
    void send(T &&msg);
    T receive();

private:
    deque<T> _queue;
    mutex _mutex;
    condition_variable _cond;
};

enum TrafficLightPhase {
    green,
    red
};

class TrafficLight : public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();

    // getters / setters
    TrafficLightPhase getCurrentPhase();

    // typical behaviour methods
    void waitForGreen();
    void simulate();

private:
    // typical behaviour methods
    void cycleThroughPhases();

    MessageQueue<TrafficLightPhase> _messageQueue;
    TrafficLightPhase _currentPhase;
    condition_variable _condition;
    mutex _mutex;
};

#endif