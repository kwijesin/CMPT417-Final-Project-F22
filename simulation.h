//CMPT 417 Final project
//25-11-2022
#ifndef SIMULATION_H
#define SIMULATION_H

#include "instances.h"
#include "heuristics.h"
#include "solvers.h"

#include <QtCore>

class Population{
    public:
    Population(const Map* map){}
    ~Population(){}

    void initialize(int popCount);	//TODO

    //A multi-threaded testing may be possible using QTConcurrent https://doc.qt.io/qt-6/qtconcurrentmap.html
    void testPopulation(Solver solver, QList<Instance> instances);	//TODO
    void keepBest(int count);	//TODO
    void crossover(int count);	//TODO
    void mutate(float mutationFactor);	//TODO
    CanonicalTDH getBest();	//TODO

    int generation;
    QList<CanonicalTDH> population;

};

void exportToCSV(Population population, QString filename);	//TODO
void exportToCSV(CanonicalTDH heuristic, QString filename);	//TODO
CanonicalTDH importFromCSV(QString filename);	//TODO

int runSimulation(){
	//input arguments, via console or GUI
	const QString MAP_NAME = "mapname.txt";
	const QString INSTANCE_NAME = "instancename.txt";
	const QString OUTPUT_NAME = "output.csv";
	const int POP_COUNT = 100;
	const int NUM_GENERATIONS = 50;
	const int TEST_SIZE = 50;
	const float MUTATION_FACTOR = 1.0;
    const int CULL_RATE = 4;	//	only the top (POP/RATE) agents are kept each generation
	
	//initialize
	Map map(MAP_NAME);
	QList<Instance> allInstances = Instance::importInstances(INSTANCE_NAME);
	QList<Instance> testInstances = Instance::getRandomTestSuite(allInstances, TEST_SIZE);
	Population population(&map);
	Solver solver = AStar();

	//save initial state and run the first generation
    population.initialize(POP_COUNT);
	exportToCSV(population, OUTPUT_NAME);
	population.testPopulation(solver, testInstances);
	exportToCSV(population, OUTPUT_NAME);
	
	while(population.generation <= NUM_GENERATIONS){
		//do the genetic part
		population.keepBest(POP_COUNT/CULL_RATE);
		population.crossover(POP_COUNT);
		population.mutate(MUTATION_FACTOR);
		
		//generate test suite
		testInstances = Instance::getRandomTestSuite(allInstances, TEST_SIZE);

		//solve and save data
		population.testPopulation(solver, testInstances);
		exportToCSV(population, OUTPUT_NAME);
	}

    return 1;
};

#endif

//Notes
/*
Data to gather
	per generation:
		Per agent:
			agent score (for making graph)
			agent K list (for making heatmaps etc)

Variables to change
	Map / Map complexity
	Population count
	Mutation factor (maybe make it anneal using a function like "float mutationFactorByGeneration(int generation)")
	Number of generations
	Test suite size
	Type of solver
	Culling rate

So far I've been using QList and QString from the QT Library
QT requires the QT creator IDE, but has additional framework for displaying results / using a GUI

These can be replaced with std::vector and std::string respectively, for native C++
*/
