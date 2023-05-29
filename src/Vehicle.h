#ifndef VEHICLE_H
#define VEHICLE_H

#include "TrafficObject.h"

using namespace std;

// forward declarations to avoid include cycle
class Street;
class Intersection;

class Vehicle : public TrafficObject, public enable_shared_from_this<Vehicle>
{

private:
    // typical behaviour methods
    void drive();

    shared_ptr<Street> _currStreet;            // street on which the vehicle is currently on
    shared_ptr<Intersection> _currDestination; // destination to which the vehicle is currently driving
    double _posStreet;                              // position on current street
    double _speed;                                  // ego speed in m/s

public:
    // constructor / desctructor
    Vehicle() {
        _currStreet = nullptr;
        _posStreet = 0.0;
        _type = ObjectType::objectVehicle;
        _speed = 200; // m/s
    }

    // getters / setters
    void setCurrentStreet(shared_ptr<Street> street) 
    { 
        _currStreet = street;
    }
    
   // void setCurrentDestination(shared_ptr<Intersection> destination);

    void setCurrentDestination(shared_ptr<Intersection> destination) {
        // update destination
        _currDestination = destination;

        // reset simulation parameters
        _posStreet = 0.0;
    }

    // typical behaviour methods
    void simulate();

    // miscellaneous
    shared_ptr<Vehicle> get_shared_this() { 
        return shared_from_this();
    }
};

#endif