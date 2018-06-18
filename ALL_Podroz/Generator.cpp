#include "Generator.h"

void generateGraph(Graph& G, int citiesCount, int roadsCount, int gasPrice)
{
	int grCount = rand() % (citiesCount/10);
	int grSize = citiesCount / grCount;
	int maxCF = rand() % DEF_MAX_CF + 1;
	int minCF = rand() % maxCF;
	int maxRF = rand() % DEF_MAX_RF + 1;
	int minRF = rand() % maxCF;
	int maxL = rand() % DEF_MAX_LEN + 1;
	int minL = rand() % maxL + 1;
	generateGraph(G, citiesCount, roadsCount, gasPrice, grCount, grSize, maxCF, minCF, maxRF, minRF, maxL, minL);
}

void generateGraph(Graph& G, int citiesCount, int roadsCount, int gasPrice, int groupsCount, int groupSize,
	int minCF, int maxCF, int minRF, int maxRF, int minL, int maxL)
{
	G.gasPrice = gasPrice;

	// Generate cities
	string baseName = "M";
	for (int i = 0; i < citiesCount; ++i)
	{
		int fee = rand() % (maxCF + 1) + minCF;
		string name = baseName + to_string(i);
		G.cities.insert({ name, City(name, fee) });
	}
	
	//Generate roads
	int roadsCreated = 0;
	int maxRoads = (citiesCount * citiesCount - citiesCount) / 2; // maximum number of edges in the graph
	if (roadsCount > maxRoads)
	{
		roadsCount = maxRoads;
	}
	int entriesLeft = maxRoads;
	for (int i = 0; i < citiesCount && roadsCount != roadsCreated; ++i)
	{
		for (int j = 0; j < i && roadsCreated != roadsCount; ++j) // since the graph is undirected calculating half of the incidence matirx is enough
		{
			if ((entriesLeft + roadsCreated - roadsCount) < 0 ||
				(rand() % 2) == 1)
			{ // cities i,j are adjancent
				int fee = rand() % (maxRF + 1) + minRF;
				int len = rand() % (maxL + 1) + minL;
				string name1 = baseName + to_string(i);
				string name2 = baseName + to_string(j);
				City& c1 = G.cities[name1];
				City& c2 = G.cities[name2];

				Road road(&c1, &c2, fee, len);

				G.roads.insert(Road(&c1, &c2, fee, len));
				c1.roads.push_back(road);
				c2.roads.push_back(road);
				roadsCreated++;
			}
			entriesLeft--;
		}
	}

	//Generate Partnership Groups
	unordered_map<int, int> changedValues; // maps radnomized cities number to nubmers left undrfated - this allows to randomize without duplictes
	int rand_max = citiesCount;
	int id = City::NO_PARTNERS_ID +1; // group id
	for (int i = 0; i < groupsCount && rand_max > 0; ++i)
	{
		PartnersGroup group(id);
		for (int j = 0; j < groupSize && rand_max > 0; j++)
		{
			int cityNo; // city number
			int cidx = rand() % rand_max + 1;
			// check for duplicate and get actual city number
			auto duplicate_it = changedValues.find(cidx);
			auto max_it = changedValues.find(rand_max);
			if (duplicate_it != changedValues.end()) {// duplicate
				cityNo = duplicate_it->second - 1;
			}
			else { // no duplicate
				cityNo = cidx - 1;
			}
			if (max_it != changedValues.end())
			{// current rand_max was changed
				changedValues[cidx] = max_it->second;
			}
			else
			{ // current rand max was not chaneg
				changedValues[cidx] = rand_max;
			}
			rand_max--; // decrease random numbers range
			string cname = baseName + to_string(cityNo);
			group.addCity(G.cities[cname]);
		}
		G.partnersMap.insert({ id, group });
		id++;
	}
}