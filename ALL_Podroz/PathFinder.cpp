#pragma once
#include "PathFinder.h"

PathFinder::PathFinder(Graph* graph)
{
	G = graph;
}
/**
Finds shortest path from source to target
*/
Path PathFinder::findShortestPath(City& source, City& target)
{
	return spDijkstra(source, target, set<int>(), INT_MAX, false);
}
/**
Find shortest path using Dijkstra's algorithm.
Parameters:
source, target - source and target cities
lenLimit - maximal path lenght - search stops if popped City cost > lenLimit
*/
Path PathFinder::spDijkstra(City& source, City& target, set<int>unlockedGroups, int costLimit, bool nestedSearch)
{
	predecessors.clear(); // clear predecessors map set in previous search
	CityMap visitedCities; //  unordered_map<City*, int> mapped value is smallest known cost from source to key
	CityPriorityQ openCities;

	CityCost src = CityCost(0, &source);

	openCities.push(src);
	CityCost currentCC = src;
	while ((currentCC.city != &target) &&
		(!openCities.empty()))
	{
		currentCC = openCities.top();
		openCities.pop(); //  pop only removes top element without returning it
		City* city = currentCC.city;
		int cost = currentCC.cost;
	
		//Check if the city has cheaper partners
		vector<City*>& cheaperPartners = G->partnersWithSmallerFee(city, cost);
		Path *cheaperPath = nullptr;
		if (!currentCC.unlocked && !cheaperPartners.empty() && city != &source && !(nestedSearch && city == &target))
		{// Search for cheaper path when current cost includes fee and potential cheaper cities exists
			//Iterate over potential cheaper partners
			for (vector<City*>::iterator partner = cheaperPartners.begin(); partner != cheaperPartners.end(); ++partner)
			{
				//Look for cheaper path forth
				City *partnerCity = *partner;
				Path candidate;
				set<int> unlockedForth;
				PathFinder pfForth(G); // use new path finder so that predecessors map will not be overwritten
				City *prevCity = predecessors[city->name].pathTarget();
				bool newPath = false;
				//check if cheaper partner was already visited
				CityMap::iterator partnerVisitedIter = visitedCities.find(partnerCity);
				if (partnerVisitedIter != visitedCities.end())
				{//Use existing path from source to partner
					newPath = false;
					candidate = recreatePath(source, *partnerCity);
					candidate.path.pop_front();
					candidate.total_cost = visitedCities[partnerCity];
					unlockedEnRoute(source, *partnerCity, unlockedForth);
				}
				else
				{// If there is a path from source to cheaper partner it would have been popped before current city
				 // and cheaper partner city would be marked as visited.
					continue;
				}
				if (candidate.isValid())
				{
					
					// Cheaper path forth was found, now search for path back within cost limit
					PathFinder pfBack(G); // use new path finder so that predecessors map will not be overwritten
					set<int> unlockedBack(unlockedForth); //  keep track of groups unlocked along the way
					if (newPath) {
						pfForth.unlockedEnRoute(*prevCity, *partnerCity, unlockedBack);
					}
					Path candidateBack = pfBack.spDijkstra(*partnerCity, *city, unlockedBack, cost- 1 - candidate.total_cost, true); // find candidate back
					if (!candidateBack.isValid())
						continue; // no cheaper round trip path

					// valid round trip option found
					candidate.path.pop_back(); // remove cheaper partner city, so it won't be doubled on the path - it is also source on the path back
					candidate.merge(candidateBack);
					candidate.path.pop_back(); // remove city from predecessors  path
					if (newPath)
					{// if new path has to be found update total cost
						candidate.total_cost += currentCC.predecessorCost;
					}
					if (cheaperPath == nullptr) { // this is the firs cheaper path
						cheaperPath = new Path(candidate.path, candidate.total_cost);
					}
					//compare candidate with currently known cheapest path
					else if (candidate.total_cost < cheaperPath->total_cost)
					{ // upate cheaper path
						delete cheaperPath;
						cheaperPath = new Path(candidate.path, candidate.total_cost);
					}
				}
			}
		}
		if (cheaperPath != nullptr)
		{// cheaper path found, update cost and path to current city
			cost =currentCC.cost = cheaperPath->total_cost;
			Path& pred = predecessors[city->name];
			pred.path.clear();
			pred.path = cheaperPath->path;
			pred.total_cost = cheaperPath->total_cost;
		}
		
		// Only now we can check cost limit because at the begining of the iteration cheaper path may yet be found
		if (currentCC.cost > costLimit)
			break; // cost limit exceeded - stop

		//Unlock current city partners group 
		if (city->hasPartners())
		{
			unlockedGroups.insert(city->partnersGroupId);
		}
		
		if (currentCC.city == &target)
		{/*
			Path& resultPath = recreatePath(source, target);
			resultPath.total_cost = currentCC.cost;
			cout << "END: Dijkstra from " << source.name << " to " << target.name << endl;
			return resultPath;
			*/
			break; // we are done after this iteration, dont bother with checking paths from target forward
		}

		typedef City::RoadVect::iterator RoadVecIter;
		for (RoadVecIter i = city->roads.begin(); i != city->roads.end(); ++i)
		{
			Road road = *i;
			City* otherCity = road.otherCity(city);;
			if (otherCity == nullptr)
				continue;

			if (visitedCities.count(otherCity) != 1) 
			{// other city was not visited yet
				bool groupUnlocked = false;
				int otherFee = otherCity->initial_fee;
				if (unlockedGroups.count(otherCity->partnersGroupId) != 0)
				{ // Partner city from same group was visited somewhere before
					//Check if the group is unlocked on the actual path to current city
					set<int> ulPathToCurrent;
					unlockedEnRoute(source, *(currentCC.city), ulPathToCurrent); 
					if (ulPathToCurrent.count(otherCity->partnersGroupId) != 0)
					{
						otherFee = 0;
						groupUnlocked = true;
					}
				}
				int otherCost = cost + otherFee + road.cost(G->gasPrice);
				CityCost otherCC = CityCost(otherCost, otherCity, groupUnlocked);
				otherCC.predecessorCost = cost;
				
				std::pair<bool, int> includesRes = openCities.includes(otherCC);//includes returns parir (included?, priority/cost)
				if ( includesRes.first && (includesRes.second > otherCC.cost ))
				{// other is present in openCities but with higher cost - update entry in openCities heap
					CityCost oldCC(includesRes.second, otherCC.city);
					openCities.updatePriority(oldCC, otherCC);
					setPredecessor(otherCity, city);
				}
				else if (!includesRes.first){// other city not in Q insert it with current cost
					openCities.push(otherCC);
					setPredecessor(otherCity, city);
				}
			}
		}
		visitedCities[city] = cost; // mark city as visited
	}
	if (currentCC.city == &target && currentCC.cost <= costLimit)
	{// Path to target found, return it
		Path& resultPath = recreatePath(source, target);
		resultPath.total_cost = currentCC.cost;
		return resultPath;
	}
	//path not found within cost limit, or doesnt exist at all
	return Path(-1); // return a dummy path
}

Path PathFinder::recreatePath(const City& source, const City& target)
{
	Path resultPath;
	City *currentCity = (City*) &target;
	resultPath.path.push_back(currentCity);
	while (currentCity != nullptr && currentCity != &source)
	{
		Path pred = predecessors[currentCity->name];
		resultPath.reversedMerge(pred);
		currentCity = predecessors[currentCity->name].pathSource();
	}

	return resultPath;
}

/**
Adds entry to predecessors map 
*/
void PathFinder::setPredecessor(City* keyCity, City* predCity)
{
	if(predecessors.count(keyCity->name) != 0)
		predecessors[keyCity->name].path.clear();

	predecessors[keyCity->name].path.push_back(predCity);
}

/**
Inserts partner groups ids on the shortest path from source to target into result  
*/
void PathFinder::unlockedEnRoute(const City& source, const City& target, set<int>& result)
{
	Path aPath = recreatePath(source, target);
	for (Path::CityList::iterator i = aPath.begin(); i != aPath.end(); ++i)
	{
		City* c = *i;
		if (c->hasPartners())
		{
			result.insert(c->partnersGroupId);
		}
	}
}