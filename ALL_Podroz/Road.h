#pragma once
/**
AUTHOR: Piotr S³ysz
VERSION: 1.0
DATE: May 2017

Road class implemetation. Road objects are used to represent edges
in cities and roads network.
*/
#include "City.h"
#include <utility>
#include <unordered_map>

class City;
using namespace std;
class Road 
{
public:
	int fee; // drive-thru fee
	int length; // number of hours needed to traverse the road
	City *city1;
	City *city2;

	/**
		Constructor method
		Params:
			city1, city2, fee, length
	*/
	Road(City* c1, City* c2, int f, int l);
	/**
		Comparison operator - roads are compared on cities values
	*/
	bool operator==(const Road& road) const;

	/**
		Given a city returns pointer to city on the other end of this road
	*/
	City* otherCity(City* city) const;

	/**
		Given cost of driving for an hour returns total cost of driving down this road
		Parameters:
			hCost - cost of driving for an hour
	*/
	int cost(int hCost);
};

