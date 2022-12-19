#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <QObject>
#include <QRandomGenerator>
#include "simulation.h"

void Population::initialize(int popCount, int k)
{
    for (int i = 0; i < popCount; i++)
    {
        CanonicalTDH tdh = CanonicalTDH(0, 0);
        tdh.setK(k);
        tdh.randomizeNodes(*map);
        population.append(tdh);
    }

    generation = 0;
}

void Population::testPopulation(Solver* solver, QList<Instance> instances)
{
    for (int i = 0; i < population.length(); i++)
    {
        CanonicalTDH* tdh = &(population.data()[i]);
        onTestStarted(i);
        tdh->calculateTDH(*map);
        tdh->score = 0;
        int numSolved = 0;
        int largestNumExpansions = 0;
        for(int j = 0; j < instances.length(); j++){
            QList<Node> path;
            int numExpansions = 0;
            bool solved = solver->solve(*map, instances.at(j), &path, &numExpansions, tdh);
            if(solved){
                numSolved++;
            }
            if(numExpansions > largestNumExpansions){
                largestNumExpansions = numExpansions;
            }
            tdh->score += numExpansions;
        }
        tdh->score += (largestNumExpansions * (instances.count() - numSolved));
        tdh->generation++;
        //tdh->score = tdh->score / instances.count();
    }
    generation++;
}

void Population::keepBest(int count)
{
    QList<CanonicalTDH> newPopulation;

    // Push all members of the population onto a max heap
    auto cmp = [](CanonicalTDH a, CanonicalTDH b) { return a.score > b.score; };
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
    while(newPopulation.length() < count){
        // pick two random members of the population
        int a = QRandomGenerator::global()->bounded(population.length());
        int b = QRandomGenerator::global()->bounded(population.length());
        while (a == b){
            b = QRandomGenerator::global()->bounded(population.length());
        }

        //add the children of those members
        QList<CanonicalTDH> children = population.at(a).crossover(population.at(b));
        newPopulation.append(children.at(0));
        if(newPopulation.length() != count)
            newPopulation.append(children.at(1));
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

void Population::onTestStarted(int testN)
{
    return;
}
void QTPopulation::onTestStarted(int testN)
{
    emit reportTestStarted(testN);
}

void exportToCSV(Population population, QString filename)
{
    QFile file(filename);
    bool isNew = !file.exists();
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        return;
    QTextStream fileOut(&file);
    if(isNew)
        fileOut << "index,generation,score,k" << "\n";
    QList<CanonicalTDH>::iterator i;
    int ind = 0;
    for (i = population.population.begin(); i != population.population.end(); i++)
    {
        fileOut << i->toCSVString(ind) << '\n';
        ind++;
    }
    file.close();
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
    population.initialize(pop, 20);
    exportToCSV(population, outputName);
    population.testPopulation(&solver, testInstances);
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
        population.testPopulation(&solver, testInstances);
        exportToCSV(population, outputName);
    }

    return 1;
}

void SimulationThread::run(){
    Map map(mapName);
    QList<Instance> allInstances = Instance::importInstances(instanceName);
    QList<Instance> testInstances;
    if(instanceName == "random")
        testInstances = Instance::generateRandomTestSuite(map, testCount);
    else
        testInstances = Instance::getRandomTestSuite(allInstances, testCount);
    population.map = &map;
    AStar solver = AStar();
    QObject::connect(&population, SIGNAL(reportTestStarted(int)), this, SIGNAL(reportProgressTest(int)));

    //save initial state and run the first generation
    population.initialize(pop, k);
    //exportToCSV(population, outputName);
    population.testPopulation(&solver, testInstances);
    exportToCSV(population, outputName);
    emit reportProgress(population.generation);

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
        population.testPopulation(&solver, testInstances);
        exportToCSV(population, outputName);

        emit reportProgress(population.generation);
    }
}
