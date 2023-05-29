#include <iostream>
#include <random>
#include "Street.h"
#include "Intersection.h"
#include "Vehicle.h"

using namespace std;

void Vehicle::simulate()
{
    // launch drive function in a thread
    threads.emplace_back(thread(&Vehicle::drive, this));
}

// virtual function which is executed in a thread
void Vehicle::drive()
{
    // print id of the current thread
    unique_lock<mutex> lck(_mtx);
    cout << "Vehicle #" << _id << "::drive: thread id = " << this_thread::get_id() << endl;
    lck.unlock();

    // initalize variables
    bool hasEnteredIntersection = false;
    double cycleDuration = 1; // duration of a single simulation cycle in ms
    chrono::time_point<chrono::system_clock> lastUpdate;

    // init stop watch
    lastUpdate = chrono::system_clock::now();
    for ( ; true ;)
    {
        // sleep at every iteration to reduce CPU usage
        this_thread::sleep_for(chrono::milliseconds(1));

        // compute time difference to stop watch
        long timeSinceLastUpdate = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - lastUpdate).count();
        if (timeSinceLastUpdate >= cycleDuration)
        {
            // update position with a constant velocity motion model
            _posStreet += _speed * timeSinceLastUpdate / 1000;

            // compute completion rate of current street
            double completion = _posStreet / _currStreet->getLength();

            // compute current pixel position on street based on driving direction
            shared_ptr<Intersection> i1, i2;
            i2 = _currDestination;
            i1 = i2->getID() == _currStreet->getInIntersection()->getID() ? _currStreet->getOutIntersection() : _currStreet->getInIntersection();

            double x1, y1, x2, y2, xv, yv, dx, dy, l;
            i1->getPosition(x1, y1);
            i2->getPosition(x2, y2);
            dx = x2 - x1;
            dy = y2 - y1;
            l = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (x1 - x2));
            xv = x1 + completion * dx; // new position based on line equation in parameter form
            yv = y1 + completion * dy;
            this->setPosition(xv, yv);

            // check wether halting position in front of destination has been reached
            if (completion >= 0.9 && !hasEnteredIntersection)
            {
                // request entry to the current intersection (using async)
                auto ftrEntryGranted = async(&Intersection::addVehicleToQueue, _currDestination, get_shared_this());

                // wait until entry has been granted
                ftrEntryGranted.get();

                // slow down and set intersection flag
                _speed /= 10.0;
                hasEnteredIntersection = true;
            }

            // check wether intersection has been crossed
            if (completion >= 1.0 && hasEnteredIntersection)
            {
                // choose next street and destination
                vector<shared_ptr<Street>> streetOptions = _currDestination->queryStreets(_currStreet);
                shared_ptr<Street> nextStreet;
                if (streetOptions.size() > 0)
                {
                    // pick one street at random and query intersection to enter this street
                    random_device rd;
                    mt19937 eng(rd());
                    uniform_int_distribution<> distr(0, streetOptions.size() - 1);
                    nextStreet = streetOptions.at(distr(eng));
                }
                else
                {
                    // this street is a dead-end, so drive back the same way
                    nextStreet = _currStreet;
                }
                
                // pick the one intersection at which the vehicle is currently not
                shared_ptr<Intersection> nextIntersection = nextStreet->getInIntersection()->getID() == _currDestination->getID() ? nextStreet->getOutIntersection() : nextStreet->getInIntersection(); 

                // send signal to intersection that vehicle has left the intersection
                _currDestination->vehicleHasLeft(get_shared_this());

                // assign new street and destination
                this->setCurrentDestination(nextIntersection);
                this->setCurrentStreet(nextStreet);

                // reset speed and intersection flag
                _speed *= 10.0;
                hasEnteredIntersection = false;
            }

            // reset stop watch for next cycle
            lastUpdate = chrono::system_clock::now();
        }
    } // eof simulation loop
}
