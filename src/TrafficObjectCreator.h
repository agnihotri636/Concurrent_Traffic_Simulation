#ifndef TRAFFICOBJECTCREATOR_H
#define TRAFFICOBJECTCREATOR_H

#include "Street.h"
#include "Vehicle.h"
#include "Intersection.h"
using namespace std;

class TrafficObjectCreator {
public:
    // Declare member functions
     void createTrafficObjects_Paris(vector<shared_ptr<Street>> &streets, vector<shared_ptr<Intersection>> &intersections, vector<shared_ptr<Vehicle>> &vehicles, string &filename, int nVehicles);
    
     void createTrafficObjects_NYC(vector<shared_ptr<Street>> &streets, vector<shared_ptr<Intersection>> &intersections, vector<shared_ptr<Vehicle>> &vehicles, string &filename, int nVehicles);

     void createTrafficObjects_FSU(vector<shared_ptr<Street>> &streets, vector<shared_ptr<Intersection>> &intersections, vector<shared_ptr<Vehicle>> &vehicles, string &filename, int nVehicles);

};

#endif // TRAFFICOBJECTCREATOR_H
