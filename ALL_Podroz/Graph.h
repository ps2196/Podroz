#pragma once
/**
AUTHOR: Piotr S³ysz
VERSION: 1.0
DATE: May 2017

City-Road network representation.
*/
#include <unordered_set>
#include <unordered_map>
#include <queue> // implements priority_queue
//#include<iostream>
#include <fstream>
#include <string>
#include "City.h"
#include "Road.h"
#include "PartnersGroup.h"
#include "RoadHasher.h"
#include "StringSplitter.h"



using namespace std;

class Graph
{
public:
	typedef unordered_map<string, City> CityMap;
	typedef unordered_set<Road, RoadHash> RoadSet;
	typedef unordered_map<int, PartnersGroup> PartnersGroupMap;

	CityMap cities;// hash map keyed on city name
	RoadSet roads;
	PartnersGroupMap partnersMap;
	int gasPrice; // cost per hour

	~Graph()
	{
		cities.clear();
		roads.clear();
		partnersMap.clear();
	}
	Graph()
	{
		gasPrice = 0;
	}

	Graph(CityMap v, RoadSet e, int gp)
	{
		cities = v;
		roads = e;
		gasPrice = gp;
	}
	/**
		Constructor method reads Graph definition from file.
		
		Parameters:
			FNAME - path to  file with graph definition

		Expected file format:
			an integer - cost of drive per hour
			|nodes| lines with ctites definitions : CITY_NAME;ENTERANCE_FEE
			<empty line>
			|edges| lines with edges definition: CITY1_NAME;CITY2_NAME;FEE;LENGHT
			<empty line>
			Lists of partner cities. Each group defined in a separate line. Cities names are ; delimited.

		NOTE: This method does not check if file format is correct
	*/
	Graph(string fname)
	{

		ifstream instream(fname);
		try
		{
			string line;
			// Gas price
			getline(instream, line);
			gasPrice = stoi(line);
			//Cities
			while (getline(instream, line))
			{
				if (line.empty()) { break; } // move to roads
		
				vector<string> name_fee = splitWithDelimiter(line, ';');
				City c(name_fee[0], stoi(name_fee[1]));
				cities.insert({ name_fee[0], c });
				City& ct = cities[c.name];
			}
			//Roads
			while (getline(instream, line))
			{
				if (line.empty()) { break; }
				
				vector<string> params = splitWithDelimiter(line, ';');
				City& city1 = cities[params[0]];
				City& city2 = cities[params[1]];
				Road road(&city1, &city2, stoi(params[2]), stoi(params[3]));
				
				roads.insert(road);
				city1.roads.push_back(road);
				city2.roads.push_back(road);
			}
			//Partner cities
			int groupId = City::NO_PARTNERS_ID + 1;
			while (getline(instream, line))
			{
				vector<string> names = splitWithDelimiter(line, ';');
				struct PartnersGroup group(groupId);
				for (int i = 0; i < (int)names.size(); ++i)
				{
					City& aCity = cities[names[i]];
					if (i == 0)
						group.minimalFee = aCity.initial_fee;
					group.cities.insert(&aCity);
					aCity.partnersGroupId = group.id;
					if (aCity.initial_fee < group.minimalFee)
						group.minimalFee = aCity.initial_fee;
				}
				partnersMap.insert({ groupId, group });
				groupId++;
			}
			instream.close();
		}
		catch (exception ex)
		{
			instream.close();
			throw exception("Cannot open file!");
		}
	}

	/**
	Given a city updates current_fee of all cities in this city partner group. New fee value is second parameter.
	*/
	void updateGroupFee(City* city, int newFee)
	{
		if (city->hasPartners())
		{
			PartnersGroup& pg = partnersMap[city->partnersGroupId];
			pg.updateFees(newFee);
		}
	}
	
	/**
	Given a city sets fees for all partner cities to initial values.
	*/
	void resetGroupFees(City* city)
	{
		if (city->hasPartners())
		{
			PartnersGroup& pg = partnersMap[city->partnersGroupId];
			pg.resetFees();
		}
	}
	/**
	For given city returns vector containing all cities from the same partner group 
	with smaller enerance fee
	*/
	vector<City*> partnersWithSmallerFee(City* city, int cost)
	{
		if (city->hasPartners())
		{
			PartnersGroup& pg = partnersMap[city->partnersGroupId];
			if (city->initial_fee > pg.minimalFee && cost  > pg.minimalFee)
				return pg.citiesWithSmallerFee(city->initial_fee);
			else // city is cheapest in its group
				return vector<City*>();
		}
		
		return vector<City*>();
	}
};