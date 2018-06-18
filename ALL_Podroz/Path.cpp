#include "Path.h"
using namespace std;

Path::Path()
{
	path = list<City*>();
	total_cost = 0;
}

Path::Path(int cost)
{
	total_cost = cost;
}

Path::Path(const CityList& p, int cost)
{
	path = CityList(p);
	total_cost = cost;
}
bool Path::isValid()
{
	return (total_cost >= 0);
}
City* Path::pathSource()
{
	if (path.empty())
		return nullptr;
	return *(path.begin());
}

City* Path::pathTarget()
{
	if (path.empty())
		return nullptr;

	CityList::iterator end = path.end();
	end--;
	return *end;
}

Path::CityList::iterator Path::begin()
{
	return path.begin();
}

Path::CityList::iterator Path::end()
{
	return path.end();
}

/**
Merge this with other. Other path is added to the end of this total cost is updated.
*/
void Path::merge( Path& other)
{
	for (CityList::iterator i = other.path.begin(); i != other.path.end(); ++i)
	{
		this->path.push_back(*i);
	}
	this->total_cost += other.total_cost;
}

/**
Merge this with reversed other
*/
void Path::reversedMerge(Path& other)
{
	if (other.path.empty())
		return;
	//for (CityList::iterator i = other.path.begin(); i != other.path.end(); ++i)
	CityList::iterator i = other.path.end();
	i--;
	for (;i != other.path.begin(); --i)
	{
		this->path.push_front(*i);
	}
	this->path.push_front(*i);
	this->total_cost += other.total_cost;
}

void Path::reverse()
{
	path.reverse();
}