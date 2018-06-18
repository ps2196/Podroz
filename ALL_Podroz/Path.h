#pragma once
/**
AUTHOR: Piotr S³ysz
VERSION: 1.0
DATE: May 2017

Path object stores ordered vector of Cities and total cost of traversing the path.
*/

#ifndef PATH_H
#define PATH_H

#include <list> 
#include "City.h"

using namespace std;

class City;

struct Path
{
public:
	typedef std::list<City*>CityList;
	CityList path;
	int total_cost;
	
	Path();
	Path(int cost);
	Path(const CityList& p, int cost);
	~Path() { path.clear(); }
	City* pathSource();
	City* pathTarget();
	void merge(Path& other);
	void reversedMerge(Path& other);
	bool isValid();
	void reverse();
	CityList::iterator begin();
	CityList::iterator end();
};

#endif // !PATH_H
