#include "Road.h"

Road::Road(City* c1, City* c2, int f, int l)
{
	city1 = c1;
	city2 = c2;
	fee = f;
	length = l;
}
bool Road::operator==(const Road& road) const
{
	return (((city1 == road.city1) && (city2 == road.city2)) ||
		((city1 == road.city2) && (city2 == road.city1)));
}

City* Road::otherCity(City* city) const
{
	if (city == nullptr)
		return nullptr;
	if (*city == *city1)
		return city2;
	else if (*city == *city2)
		return city1;
	else
		return nullptr;
}

int Road::cost(int hCost)
{
	return(fee + hCost*length);
}