#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <QRandomGenerator>
#include "simulation.h"

void Population::initialize(int popCount)
{
    for (int i = 0; i < popCount; i++)
    {
        CanonicalTDH tdh = CanonicalTDH(0, 0);
        tdh.randomizeNodes(*map);
        population.append(tdh);
    }

    generation = 0;
}

void Population::testPopulation(Solver solver, QList<Instance> instances)
{
    for (int i = 0; i < population.length(); i++)
    {
        CanonicalTDH* tdh = &(population.data()[i]);
        tdh->calculateTDH(*map);
        for(int j = 0; j < instances.length(); j++){
            int pathLen;
            solver.solve(*map, instances.at(j), &pathLen, &tdh->score, tdh);
        }
    }
    generation++;
}

void Population::keepBest(int count)
{
    QList<CanonicalTDH> newPopulation;

    // Push all members of the population onto a max heap
    auto cmp = [](CanonicalTDH a, CanonicalTDH b) { return a.score < b.score; };
    std::priority_queue<CanonicalTDH, std::vector<CanonicalTDH>, decltype(cmp)> pq(cmp);
    for (int i = 0; i < population.length(); i++)
    {
        pq.push(population.at(i));
    }

    // pop the best 'count' members of the population from the max heap
    for (int i = 0; i < count; i++)
    {
        newPopulation.append(pq.top());
        pq.pop();
    }

    population = newPopulation;
}

void Population::crossover(int count)
{
    QList<CanonicalTDH> newPopulation;
    for (int i = 0; i < count; i++)
    {
        // pick two random members of the population
        int a = QRandomGenerator::global()->bounded(population.length());
        int b = QRandomGenerator::global()->bounded(population.length());
        while (a == b){
            b = QRandomGenerator::global()->bounded(population.length());
        }

        // cross them
        CanonicalTDH childTDH = population.at(a).crossover(population.at(b));
        newPopulation.append(childTDH);
    }
    population = newPopulation;
}

void Population::mutate(float mutationFactor)
{
    for (int i = 0; i < population.length(); i++)
    {
        population.data()[i].mutateNodes(*map, mutationFactor);
    }
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
    std::ofstream f(filename.toStdString(), std::ofstream::out);
    f << "index,generation,score,k" << std::endl;
    QList<CanonicalTDH>::iterator i;
    int ind = 0;
    for (i = population.population.begin(); i != population.population.end(); i++)
    {
        f << i->toCSVString(ind).toStdString() << std::endl;
        ind++;
    }
    f.close();
}

void exportToCSV(CanonicalTDH heuristic, QString filename)
{
    QFile file(filename);
    if(!file.exists())
        return;
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream fileOut(&file);
    fileOut << heuristic.toCSVString(0) << "\n";

    file.close();
}

QList<CanonicalTDH> importFromCSV(QString filename)
{
    QList<CanonicalTDH> ret;
    QFile file(filename);
    QString line;
    if(!file.exists())
        return ret;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return ret;

    line = file.readLine();
    while(!file.atEnd()){
        line = file.readLine();
        if(line.endsWith('\n'))
            line.chop(1);
        CanonicalTDH tdh = CanonicalTDH::fromCSVString(line);
        ret.append(tdh);
    }

    file.close();
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
