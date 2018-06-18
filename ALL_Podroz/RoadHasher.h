#pragma once
#include "City.h"
#include "Road.h"

class Road;

class RoadHash {
public:
	size_t operator()(const Road& road) const;
};

class RoadPtrHash
{
public:
	size_t operator()(Road* road) const;
};
