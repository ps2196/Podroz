/*#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>
*/
#include <limits>

//#include "City.h"
//#include "Road.h"
#include "PathFinder.h"
#include "Generator.h"
#include <iomanip>
#include <ctime>
//#include "Generator.cpp"

string in_fname, out_fname;
const int STANDARD = 0, TIME_TESING = 1, GENERATOR = 2, HELP = 3, UNSET = -1;
int mode = UNSET; // Execution mode

//Generator parameters, set in GENERATOR mode - optional params are initialized with default values
int citiesCount, roadsCount, gasPrice; // Required
int groupsCount = 0 ,groupSize =0; //  group params (Optional)
int minCF = 0, maxCF = DEF_MAX_CF; // city params (Optional)
int minRF = 0 , maxRF = DEF_MAX_RF , minL = 1, maxL = 24; // road params (Optional)

void parse_args(int argc, char** argv);
void parseGeneratorParams(int, char**, int&);
void printHelp();
void executeStandardMode();
void executeGeneratorMode();
void executeTimeTests();
void printGraph( Graph&);
void printResult( Path&, City&, City&);

using std::cout;
void main(int argc, char** argv)
{
	try {
		if (argc == 1)
			mode = STANDARD;
		else
		{
			parse_args(argc, argv);
			if (mode == UNSET)
				mode = STANDARD;
		}
	}
	catch (exception ex) {
		cout << ex.what() << endl;
		cout << "Run with -help to see available parameters\n";
		return;
	}
	try {
		switch (mode)
		{
		case HELP:
		{
			printHelp();
			return;
		}
		case STANDARD:
		{
			executeStandardMode();
			return;
		}
		case GENERATOR:
		{
			executeGeneratorMode();
			getchar(); getchar();
			return;
		}
		case TIME_TESING:
		{
			executeTimeTests();
			return;
		}
		}
	}
	catch (exception ex) {
		cout << ex.what() << endl;
	}
	return;
}

void executeStandardMode()
{
	if (in_fname.empty())
	{
		cout << "Supply input file.\n";
		return;
	}
	Graph g(in_fname);
	printGraph(g);
	string source_name, target_name;
	cout << "Enter source city name: ";
	cin >> source_name;
	cout << "Enter target city name: ";
	cin >> target_name;
	auto sit = g.cities.find(source_name);
	auto tit = g.cities.find(target_name);
	if(sit == g.cities.end())
	{
		cout << "City " << source_name << " not found in the graph\n";
		return;
	}
	if (tit == g.cities.end())
	{
		cout << "City " << target_name << " not found in the graph\n";
		return;
	}
	
	// Source and target OK
	PathFinder pf(&g);
	Path& sp = pf.findShortestPath(sit->second, tit->second);
	printResult(sp, sit->second, tit->second);
}

/**
GENERATOR MODE - generates random graph with given parameters and finds shortest path beetween random cities
*/
void executeGeneratorMode()
{
	srand(time(NULL));
	Graph g;
	generateGraph(g, citiesCount, roadsCount, gasPrice,
		groupsCount, groupSize,
		minCF, maxCF, minRF, maxRF, minL, maxL);
	string base_name("M");
	string source_name = base_name + to_string(rand() % citiesCount);
	string target_name = base_name + to_string(rand() % citiesCount);

	City& source = g.cities[source_name];
	City& target = g.cities[target_name];
	//Run and time
	clock_t begin = clock();
	PathFinder pf(&g);
	Path& sp = pf.findShortestPath(source, target);
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	
	printGraph(g);
	printResult(sp, source, target);
	cout << "Elapsed time: " << elapsed_secs << " second(s).\n";
}

void executeTimeTests()
{
	srand(time(NULL));
	const int REPS_PER_SIZE = 10;
	const int city_step = 500;
	int max_roads = (citiesCount * citiesCount - citiesCount) / 2;
	const float road_ratio = (float)roadsCount/(float)max_roads; // road ratio in original test set 
	vector<double> times;
	double mean_time = 0.0; //mean time of test for recent size
	cout << "Roads ratio: " << road_ratio << endl;
	cout << "|V|" << "\t|E|" << "\ttime" << "\tq(N)"<<endl;
	while(mean_time < 60.0) // set mean_time limit for 1min
	{
		Graph g;
		generateGraph(g, citiesCount, roadsCount, gasPrice,
			groupsCount, groupSize,
			minCF, maxCF, minRF, maxRF, minL, maxL);
		double elapsed_sum = 0.0, max_time = 0.0;
		for (int i = 0; i < REPS_PER_SIZE; i++)
		{
			string base_name("M");
			string source_name = base_name + to_string(rand() % citiesCount);
			string target_name = base_name + to_string(rand() % citiesCount);

			City& source = g.cities[source_name];
			City& target = g.cities[target_name];

			clock_t begin = clock();
			PathFinder pf(&g);
			Path& sp = pf.findShortestPath(source, target);
			clock_t end = clock();
			double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			elapsed_sum += elapsed_secs;
			if (elapsed_secs > max_time)
				max_time = elapsed_secs;
		}
		//Calculate mean execution time
		mean_time = elapsed_sum / (float)REPS_PER_SIZE;
		times.push_back(mean_time);
		double Tn = roadsCount * log(citiesCount) * 0.00003; // teoretical time
		if(groupSize != 0 && groupsCount != 0)
			Tn = (groupsCount + 1) * (groupSize + 1) *roadsCount * log(citiesCount) * 0.00001;
		double q = (max_time * CLOCKS_PER_SEC) / Tn;
		//cout << "|V| = " << citiesCount << "|E| = " << roadsCount << "  Mean time: " << mean_time << endl;
		cout << citiesCount << "\t" << roadsCount << "\t" << mean_time<<"\t"<<q<<endl;
		// Increas problem size
		citiesCount += city_step;
		int new_max_roads = (citiesCount * citiesCount - citiesCount) / 2;
		roadsCount = (int)(new_max_roads * road_ratio);
	}
}

void printGraph(Graph& g)
{
	printf("|V| = %d, |E| = %d |P| = %d\n", g.cities.size(), g.roads.size(), g.partnersMap.size());
	if (g.cities.size() > 20)
		return; // do not print big graphs on std out
	printf("Cities:\n");
	/*cout << left<<setw(10)<<left<<"Name"
		<< setw(10) << " | Fee"
		<< setw(10) << " | GID" << endl;*/
	for (auto i = g.cities.begin(); i != g.cities.end(); ++i)
	{
		City& c = (City)i->second;
		/*cout<< left<<setw(10)<<c.name
			<<setw(10)<<" | " << c.initial_fee
			<<setw(10)<<" | "  <<  c.partnersGroupId << endl;*/
		printf("Name: %s\tIfee:%d\tCfee:%d\t deg: %d\t G_ID:%d\n", c.name.c_str(), c.initial_fee, c.current_fee, c.roads.size(), c.partnersGroupId);
	}
	cout << "\nRoads:\n";
	for (auto r = g.roads.begin(); r != g.roads.end(); ++r)
	{
		cout << "City1: " << r->city1->name << " City2: " << r->city2->name << " Fee: " << r->fee << " Length: " << r->length << endl;
	}
	for (auto grp = g.partnersMap.begin(); grp != g.partnersMap.end(); ++grp)
	{
		PartnersGroup pg = grp->second;
		cout << "Group ID: " << pg.id << " size: " << pg.cities.size() << endl;
		for (auto ct = pg.cities.begin(); ct != pg.cities.end(); ++ct)
		{
			cout << "\t" << (*ct)->name << endl;
		}
	}
}

void printResult(Path& sp, City& source, City& target)
{
	cout << "\nShortest path from " << source.name << " to " << target.name << ": \n";
	if (!sp.isValid())
	{
		cout << "Path not found.\n";
		return;
	}
	
	for (auto p = sp.path.begin(); p != sp.path.end(); ++p)
	{
		City* c = *p;
		cout << c->name << " - ";
	}
	cout << "Total cost: " << sp.total_cost << endl;
}


/*
Prints help message on std out
*/
void printHelp()
{
	cout << "Opcje uruchumienia programu :\n" <<
		"-in <nazwa_pliku>	-plik z danymi wejœciowymi\n" <<
		"-gen\t-losowo generuje instancje problemu i rozwi¹zuje go. Parmetry generatora:\n" <<
		"\t-c <liczba_miast>\n"<<
		"\t-r <liczba_dróg>\n" <<
		"\t-gp <koszt_paliwa>\n" <<
		"\t[-pc <liczba_gr_part>]\n" <<
		"\t[-ps <œr_wielkoœæ_gr_part>]\n" <<
		"\t[-mincf <min_oplata_miasto>]\n" <<
		"\t[-maxcf <max_oplata_miasto>]\n" <<
		"\t[-minrf <min_oplata_droga>]\n" <<
		"\t[-maxrf <max_oplata_droga>]\n" <<
		"\t[-minrl <min_dlugos_drogi>]\n" <<
		"\t[-maxrl <max_dlugosc_drogi>]\n" <<
		"- time - pomiary czasu dla losowo generownych instancji poroblemu\n";
}

/*
Parse command line arguments and set context
*/
void parse_args(int argc, char** argv)
{
	int i = 1; // argv[0] is program name
	while (i<argc)
	{
		if (strcmp(argv[i], "-in") == 0)
		{// Input file
			if (i == argc - 1)
				throw exception("Input file not defined!\n");
			else {
				i++;
				in_fname = string(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-out") == 0)
		{ //Output file
			if (i == argc - 1)
				throw exception("Output file not defined!\n");
			else {
				i++;
				out_fname = string(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-gen") == 0)
		{ // Generator mode
			if (i == argc - 1)
				throw exception("Missing generator parameters!\n");
			else {
				i++;
				int start_i = i;
				parseGeneratorParams(argc, argv, i);
				if (i - start_i < 6)
					throw exception("Too few parameters for generator!\n");
				mode = GENERATOR;
			}
		}
		else if (strcmp(argv[i], "-time") == 0)
		{ // Time testing mode
			mode = TIME_TESING;
			i++;
			int start_i = i;
			parseGeneratorParams(argc, argv, i);
			if (i - start_i < 6)
				throw exception("Too few parameters for generator!\n");
		}
		else if (strcmp(argv[i], "-help") == 0)
		{ // Time testing mode
			mode = HELP;
		}
		else { // Unknown parameter
			cout << argv[i];
			throw exception("Unknown parameter!\n");
		}
		i++;
	}
}
/*
Parses generator params
*/
void parseGeneratorParams(int argc, char** argv, int& i)
{
	while (i < argc)
	{
		if (strcmp(argv[i], "-c") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				citiesCount = atoi(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-r") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				roadsCount = atoi(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-gp") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				gasPrice = atoi(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-pc") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				groupsCount = atoi(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-ps") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				groupSize = atoi(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-mincf") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				minCF = atoi(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-maxcf") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				maxCF = atoi(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-minrf") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				minRF = atoi(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-maxrf") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				maxRF = atoi(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-minrl") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				minL = atoi(argv[i]);
			}
		}
		else if (strcmp(argv[i], "-maxrl") == 0)
		{
			if (i == argc - 1)
				throw exception("Generator param missing!\n");
			else
			{
				i++;
				maxL = atoi(argv[i]);
			}
		}	
		else { // arg is not a param - exit
			i--; // let arg be parsed by command parser
			return;
		} 
		i++;
	}
}

