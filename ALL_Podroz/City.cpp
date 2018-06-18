#include "City.h"

/**
Consturctor method.
Parameters:
name, initial fee
*/
City::City(string n, int init_fee)
{
	name = n;
	initial_fee = init_fee;
	current_fee = initial_fee;
	partnersGroupId = NO_PARTNERS_ID;
	//predecessors = nullptr;
}

City::City() :City("", -1) {}

/**
Returns true if city has partner cities
*/
bool City::hasPartners()
{
	return partnersGroupId != NO_PARTNERS_ID;
}

/**
Cities are compared based on a name
*/
bool City::operator==(const City& other) const
{
	return (name == other.name);
}

City::~City() {
	roads.clear();
}