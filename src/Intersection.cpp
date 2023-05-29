#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <random>

#include "Street.h"
#include "Intersection.h"
#include "Vehicle.h"

using namespace std;

/* Implementation of class "WaitingVehicles" */

int WaitingVehicles::getSize()
{
    lock_guard<mutex> lock(_mutex);

    return _vehicles.size();
}

void WaitingVehicles::pushBack(shared_ptr<Vehicle> vehicle, promise<void> &&promise)
{
    lock_guard<mutex> lock(_mutex);

    _vehicles.push_back(vehicle);
    _promises.push_back(move(promise));
}

void WaitingVehicles::permitEntryToFirstInQueue()
{
    lock_guard<mutex> lock(_mutex);

    // get entries from the front of both queues
    auto firstPromise = _promises.begin();
    auto firstVehicle = _vehicles.begin();

    // fulfill promise and send signal back that permission to enter has been granted
    firstPromise->set_value();

    // remove front elements from both queues
    _vehicles.erase(firstVehicle);
    _promises.erase(firstPromise);
}

/* Implementation of class "Intersection" */

Intersection::Intersection()
{
    _type = ObjectType::objectIntersection;
    _isBlocked = false;
}

void Intersection::addStreet(shared_ptr<Street> street)
{
    _streets.push_back(street);
}

vector<shared_ptr<Street>> Intersection::queryStreets(shared_ptr<Street> incoming)
{
    // store all outgoing streets in a vector ...
    vector<shared_ptr<Street>> outgoings;
    for (auto it : _streets)
    {
        if (incoming->getID() != it->getID()) // ... except the street making the inquiry
        {
            outgoings.push_back(it);
        }
    }

    return outgoings;
}

// adds a new vehicle to the queue and returns once the vehicle is allowed to enter
void Intersection::addVehicleToQueue(shared_ptr<Vehicle> vehicle)
{
    unique_lock<mutex> lck(_mtx);
    cout << "Intersection #" << _id << "::addVehicleToQueue: thread id = " << this_thread::get_id() << endl;
    lck.unlock();

    // add new vehicle to the end of the waiting line
    promise<void> prmsVehicleAllowedToEnter;
    future<void> ftrVehicleAllowedToEnter = prmsVehicleAllowedToEnter.get_future();
    _waitingVehicles.pushBack(vehicle, move(prmsVehicleAllowedToEnter));

    // wait until the vehicle is allowed to enter
    ftrVehicleAllowedToEnter.wait();
    lck.lock();
    cout << "Intersection #" << _id << ": Vehicle #" << vehicle->getID() << " is granted entry." << endl;
    lck.unlock();

    // if traffic light isn't green, wait for it
    if (!trafficLightIsGreen())
        _trafficLight.waitForGreen();
}

void Intersection::vehicleHasLeft(shared_ptr<Vehicle> vehicle)
{
    //cout << "Intersection #" << _id << ": Vehicle #" << vehicle->getID() << " has left." << endl;

    // unblock queue processing
    this->setIsBlocked(false);
}

void Intersection::setIsBlocked(bool isBlocked)
{
    _isBlocked = isBlocked;
    //cout << "Intersection #" << _id << " isBlocked=" << isBlocked << endl;
}

// virtual function which is executed in a thread
void Intersection::simulate() // using threads + promises/futures + exceptions
{
    // FP.6a : In Intersection.h, add a private member _trafficLight of type TrafficLight. At this position, start the simulation of _trafficLight.
    threads.emplace_back(thread(&TrafficLight::simulate, &_trafficLight));

    // launch vehicle queue processing in a thread
    threads.emplace_back(thread(&Intersection::processVehicleQueue, this));
}

void Intersection::processVehicleQueue()
{
    // print id of the current thread
    //cout << "Intersection #" << _id << "::processVehicleQueue: thread id = " << this_thread::get_id() << endl;

    // continuously process the vehicle queue
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        this_thread::sleep_for(chrono::milliseconds(1));

        // only proceed when at least one vehicle is waiting in the queue
        if (_waitingVehicles.getSize() > 0 && !_isBlocked)
        {
            // set intersection to "blocked" to prevent other vehicles from entering
            this->setIsBlocked(true);

            // permit entry to first vehicle in the queue (FIFO)
            _waitingVehicles.permitEntryToFirstInQueue();
        }
    }
}

bool Intersection::trafficLightIsGreen()
{
   if (_trafficLight.getCurrentPhase() == TrafficLightPhase::green)
       return true;
   else
       return false;
} 