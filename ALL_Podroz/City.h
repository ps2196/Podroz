#pragma once
/**
AUTHOR: Piotr S³ysz
VERSION: 1.0
DATE: May 2017

City class implemetation. City objects are used to represent vertices 
in cities and roads network.

Assuming a city is identified with name
*/

#include <string>
#include <vector>
#include "Road.h"
#include "RoadHasher.h"
//#include "Path.h"
#include <iostream>

using namespace std;
//class Road;
//struct Path;

class City
{
public:
	typedef vector<Road> RoadVect;
	static const int NO_PARTNERS_ID = 0;
	string name;
	int initial_fee; // Initial enterance fee
	int current_fee; // Enterence fee dependent on previously visited cities
	RoadVect roads; // Roads incident with this city
	int partnersGroupId;
	
	City();
	City(string n, int init_fee);
	~City();
	bool hasPartners();
	bool operator==(const City& other) const;
};
