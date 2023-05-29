#include <iostream>
#include <thread>
#include <vector>

#include "Vehicle.h"
#include "Street.h"
#include "Intersection.h"
#include "Graphics.h"
#include "TrafficObjectCreator.h"

using namespace std;

/* Main function */
int main()
{
    /* PART 1 : Set up traffic objects */

    // create and connect intersections and streets
    vector<shared_ptr<Street>> streets;
    vector<shared_ptr<Intersection>> intersections;
    vector<shared_ptr<Vehicle>> vehicles;
    TrafficObjectCreator trafficObjectCreator;
    string backgroundImg;
    int nVehicles = 6;
    //createTrafficObjects_Paris(streets, intersections, vehicles, backgroundImg, nVehicles);
    trafficObjectCreator.createTrafficObjects_NYC(streets, intersections, vehicles, backgroundImg, nVehicles);

    /* PART 2 : simulate traffic objects */

    // simulate intersection
    // for_each(intersections.begin(), intersections.end(), [](shared_ptr<Intersection> &i) {
    //     i->simulate();
    // });
    auto intersectionIt = intersections.begin();
    while (intersectionIt != intersections.end()) {
        (*intersectionIt)->simulate();
        ++intersectionIt;
    }


    // simulate vehicles
    // for_each(vehicles.begin(), vehicles.end(), [](shared_ptr<Vehicle> &v) {
    //     v->simulate();
    // });
    auto it = vehicles.begin();
    while (it != vehicles.end()) {
        (*it)->simulate();
        ++it;
    }


    /* PART 3 : Launch visualization */

    // add all objects into common vector
    // vector<shared_ptr<TrafficObject>> trafficObjects;
    // for_each(intersections.begin(), intersections.end(), [&trafficObjects](shared_ptr<Intersection> &intersection) {
    //     shared_ptr<TrafficObject> trafficObject = dynamic_pointer_cast<TrafficObject>(intersection);
    //     trafficObjects.push_back(trafficObject);
    // });
    vector<shared_ptr<TrafficObject>> trafficObjects;
    auto intersectionIterator = intersections.begin();
    while (intersectionIterator != intersections.end()) {
        shared_ptr<TrafficObject> trafficObject = dynamic_pointer_cast<TrafficObject>(*intersectionIterator);
        trafficObjects.push_back(trafficObject);
        intersectionIterator++;
    }


    // for_each(vehicles.begin(), vehicles.end(), [&trafficObjects](shared_ptr<Vehicle> &vehicles) {
    //     shared_ptr<TrafficObject> trafficObject = dynamic_pointer_cast<TrafficObject>(vehicles);
    //     trafficObjects.push_back(trafficObject);
    // });
    auto vehicleIt = vehicles.begin();
    while (vehicleIt != vehicles.end()) {
        shared_ptr<TrafficObject> trafficObject = dynamic_pointer_cast<TrafficObject>(*vehicleIt);
        trafficObjects.push_back(trafficObject);
        ++vehicleIt;
    }


    // draw all objects in vector
    Graphics *graphics = new Graphics();
    graphics->setBgFilename(backgroundImg);
    graphics->setTrafficObjects(trafficObjects);
    graphics->simulate();
}
