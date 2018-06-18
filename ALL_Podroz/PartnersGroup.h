#pragma once
#include "City.h"
#include <unordered_set>

struct PartnersGroup
{
	typedef unordered_set<City*> CitySet;
	int id; 
	CitySet cities;
	int minimalFee; // lowest enterance fee

	PartnersGroup(int anId)
	{
		id = anId;
		minimalFee = INT_MAX;
	}
	PartnersGroup() :PartnersGroup(-1) {}

	~PartnersGroup()
	{
		cities.clear();
	}

	/**
	Set current cost of each city in the group for newFee
	*/
	void updateFees(int newFee)
	{
		for (CitySet::iterator i = cities.begin(); i != cities.end(); ++i)
		{
			City* c = *i;
			c->current_fee = newFee;
		}
	}
	/**
	Set current fee to initial value for each city in the group
	*/
	void resetFees()
	{
		for (CitySet::iterator i = cities.begin(); i != cities.end(); ++i)
		{
			City* c = *i;
			c->current_fee = c->initial_fee;
		}
	}

	/**
	For given fee returns all cities with smaller fee
	*/
	vector<City*> citiesWithSmallerFee(int fee)
	{
		vector<City*> result;
		for(CitySet::iterator i = cities.begin(); i != cities.end(); ++i)
		{	
			if ((*i)->initial_fee < fee)
				result.push_back(*i);
		}

		return result;
	}
	/**
	Adds new city into the group
	*/
	void addCity(City& city)
	{
		if (city.initial_fee < minimalFee)
			minimalFee = city.initial_fee;
		city.partnersGroupId = id;
		cities.insert(&city);
	}
};