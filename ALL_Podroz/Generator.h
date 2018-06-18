#pragma once
/**
AUTHOR: Piotr S³ysz
VERSION: 1.0
DATE: June 2018

Generator of city-road network, parametrized with:
* number of cities
* number of roads
* gas price
- average size of partner group
- max/min fee
- min/max len


*/
#ifndef GENERATOR_H
#define GENERATOR_H

#include <cstdlib>
#include <time.h>
#include "Graph.h"

//Default generator values
#define DEF_MAX_CF 1000
#define DEF_MAX_RF 1000
#define DEF_MAX_LEN 500

void generateGraph(Graph& G, int citiesCount, int roadsCount, int gasPrice);
void generateGraph(Graph& G, int citiesCount, int roadsCount, int gasPrice, int groupsCount,  int groupSize,
	int minCF, int maxCF, int minRF, int maxRF, int minL, int maxL);


#endif // !GENERATOR_H