#pragma once
/**
AUTHOR: Piotr S³ysz
VERSION: 1.0
DATE: May 2017

Implements algorithm for finding shortest path in given Graph

*/
#include <queue> // implements priority_queue
#include <set>
#include <functional>
#include <iostream> //  DEBUG
#include"Graph.h"
#include "Path.h"
#include "Heap.h"
#include "Heap.cpp"

using namespace std;

/**

CityCost is used in a priority_queue used in Dijkstra algorithm

*/
struct CityCost {
	int cost;
	int predecessorCost;
	bool unlocked; // true means that cost was calculated when partner group for this city was unlocked 
				  // search for cheaper path should not be run for when this is true.
	City* city;

	CityCost() : cost(-1), city(nullptr){}
	CityCost(int cst, City* c)
	{
		cost = cst;
		unlocked = false;
		predecessorCost = 0;
		city = c;
	}
	CityCost(int cst, City* c, bool ul)
	{
		cost = cst;
		unlocked = ul;
		predecessorCost = 0;
		city = c;
	}
	
	bool operator==(const CityCost& other) const
	{
		return (*city == *(other.city));
	}
	bool operator!=(const CityCost& other) const
	{
		return !(*this == other);
	}
	// invert comparison operator to get minimizing heap
	bool operator<(const CityCost& other) const
	{
		if (this->city->name == other.city->name) {
			return false;
		}
		// diffrent cities
		if (cost == other.cost) {
			return (city->name < other.city->name); // if costs are equla compare names lexicogrphically
		}
		else {
			return (cost < other.cost);
		}
	}
	bool operator<=(const CityCost& other) const
	{
		return (cost < other.cost) || (*this == other);
	}
	bool operator>(const CityCost& other) const
	{
		if (this->city->name == other.city->name)
			return false;
		// diffrent cities
		if (cost == other.cost)
			return (city->name > other.city->name); // if costs are equla compare names lexicogrphically
		return (cost > other.cost);
	}
	bool operator>=(const CityCost& other) const
	{
		return (cost > other.cost) || (*this == other);
	}

	// Method required by heap
	int priority() const { return cost; }
	void setPriority(int np) { cost = np;}
	string key() const  { return city->name; }
};


/**
Implements path finding algorithm
*/
class PathFinder {
private:
	typedef unordered_map<City*, int> CityMap;
	typedef Heap<CityCost> CityPriorityQ;
	typedef unordered_map<string, Path> PredecessorsMap;
	Graph* G;
	PredecessorsMap predecessors; // Predecessors map keyed on city name

	void setPredecessor(City* keyCity, City* predCity);
	Path recreatePath(const City& source, const City& target);
	void unlockedEnRoute(const City& source, const City& target, set<int>& result);
public:
	PathFinder(Graph* graph);
	/**
	Finds shortest path from source to target
	*/
	~PathFinder(){
		predecessors.clear();
	}

	Path findShortestPath(City& source,  City& target);
	/**
	Find shortest path using Dijkstra's algorithm.
	Parameters:
		source, target - source and target cities
		unlockedGroups - groups unlocked until this point
		lenLimit - maximal path lenght - search stops if popped City cost > lenLimit
		nestesSearch - true means search was called from anoter search - poping target node should stop imidiatley
	*/
	Path spDijkstra(City& source, City& target, set<int> unlockedGroups, int costLimit, bool nestedSearch);
	
	
};