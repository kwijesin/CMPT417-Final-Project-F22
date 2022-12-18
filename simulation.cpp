#include <iostream>
#include <fstream>
#include "simulation.h"

Population::Population(const Map* map) : map(map) {}

void Population::initialize(int popCount)
{
    for (int i = 0; i < popCount; i++)
    {
        // 
        population.append(CanonicalTDH::CanonicalTDH(0, 0));
    }

    generation = 0;
}

void Population::testPopulation(Solver solver, QList<Instance> instances)
{
    QList<Instance>::iterator i;
    for (i = instances.begin(); i != instances.end(); i++)
    {
        Solver::solve(map, *i, &(population.at(i).nodes), &(population.at(i)));
    }

    generation++;
}

void Population::keepBest(int count)
{
    CanonicalTDH best = getBest();
    
}

void Population::crossover(int count)
{
    //TODO
}

void Population::mutate(float mutationFactor)
{
    //TODO
}

CanonicalTDH Population::getBest()
{
    CanonicalTDH best;
    for(int i = 0; i < population.length(); i++)
        if(population.at(i).score > best.score)
            best = population.at(i);
    return best;
}

void exportToCSV(Population population, QString filename)
{
    std::ofstream f(filename);
    f << "index,generation,score,k" << std::endl;
    QList<CanonicalTDH>::iterator i;
    for (i = population.population.begin(); i != population.population.end(); i++)
    {
        f << i << "," << i->generation << "," << i->score << "," << i->nodes.count() << std::endl;
    }
    f.close();
}

void exportToCSV(CanonicalTDH heuristic, QString filename)
{
    
}

QList<CanonicalTDH> importFromCSV(QString filename)
{
    QList<CanonicalTDH> ret;

    std::ifstream f(filename);
    std::getline(f, line); // skip the first line
    std::string line, col_val;
    if (f.good())
    {
        std::getline(f, line);
        while (std::getline(f, line))
        {
            CanonicalTDH tdh;
            std::stringstream ss(line);
            std::getline(ss, col_val, ','); // skip the index
            std::getline(ss, col_val, ','); // get the generation in col_val
            tdh.generation = col_val.toInt();
            std::getline(ss, col_val, ','); // get the score in col_val
            tdh.score = col_val.toInt();
            std::getline(ss, col_val, ','); // skip the number of nodes

            ret.append(tdh);
        }
    }
    f.close();
    return ret;
}

int runSimulation(){
    const QString MAP_NAME = "mapname.txt";
    const QString INSTANCE_NAME = "instancename.txt";
    const QString OUTPUT_NAME = "output.csv";
    const int POP_COUNT = 100;
    const int NUM_GENERATIONS = 50;
    const int TEST_SIZE = 50;
    const float MUTATION_FACTOR = 1.0;
    const int CULL_RATE = 4;	//	only the top (POP/RATE) agents are kept each generation

    return runSimulation(MAP_NAME, INSTANCE_NAME, OUTPUT_NAME, POP_COUNT, NUM_GENERATIONS, TEST_SIZE, CULL_RATE, MUTATION_FACTOR);
};

int runSimulation(QString mapName, QString instanceName, QString outputName, int pop, int generations, int testCount, int cullRate, float mutation)
{
    Map map(mapName);
    QList<Instance> allInstances = Instance::importInstances(instanceName);
    QList<Instance> testInstances = Instance::getRandomTestSuite(allInstances, testCount);
    Population population(&map);
    Solver solver = AStar();

    //save initial state and run the first generation
    population.initialize(pop);
    exportToCSV(population, outputName);
    population.testPopulation(solver, testInstances);
    exportToCSV(population, outputName);

    while(population.generation <= generations){
        //do the genetic part
        population.keepBest(pop/cullRate);
        population.crossover(pop);
        population.mutate(mutation);

        //generate test suite
        if(instanceName == "random")
            testInstances = Instance::generateRandomTestSuite(map, testCount);
        else
            testInstances = Instance::getRandomTestSuite(allInstances, testCount);

        //solve and save data
        population.testPopulation(solver, testInstances);
        exportToCSV(population, outputName);
    }

    return 1;
}

void SimulationThread::run(){
    Map map(mapName);
    QList<Instance> allInstances = Instance::importInstances(instanceName);
    QList<Instance> testInstances = Instance::getRandomTestSuite(allInstances, testCount);
    Population population(&map);
    Solver solver = AStar();

    //save initial state and run the first generation
    population.initialize(pop);
    exportToCSV(population, outputName);
    population.testPopulation(solver, testInstances);
    exportToCSV(population, outputName);

    while(population.generation <= generations){
        //do the genetic part
        population.keepBest(pop/cullRate);
        population.crossover(pop);
        population.mutate(mutation);

        //generate test suite
        testInstances = Instance::getRandomTestSuite(allInstances, testCount);

        //solve and save data
        population.testPopulation(solver, testInstances);
        exportToCSV(population, outputName);

        msleep(QRandomGenerator::global()->bounded(500, 1000)); //TESTING sleeps for 0.5-1 second to simulate calculation time

        emit reportProgress(population.generation);
    }
}
