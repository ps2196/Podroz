#include "RoadHasher.h"

size_t RoadHash::operator()(const Road& road) const
{
	return (hash<string>()(road.city1->name) ^ hash<string>()(road.city2->name));
}

size_t RoadPtrHash::operator()(Road* road) const
{
	return (hash<string>()(road->city1->name) ^ hash<string>()(road->city2->name));
}
