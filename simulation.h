//CMPT 417 Final project
//25-11-2022
#ifndef SIMULATION_H
#define SIMULATION_H

#include "instances.h"
#include "heuristics.h"
#include "solvers.h"

#include <QtCore>
#include <QThread>

class Population{
    public:
    Population(){};
    Population(Map* m){map = m;}
    ~Population(){}

    /*!
     * \brief initialize
     * populates the canonical TDH list with popcount items. Each Heuristic is at generation 0 with randomized starts
     */
    void initialize(int popCount, int k);

    /*!
     * \brief testPopulation
     * tests a population on a list of instances using a solver. The generation will be incremented after this function ends.
     * Possibly could use multithreading using QTConcurrent https://doc.qt.io/qt-6/qtconcurrentmap.html
     */
    void testPopulation(Solver* solver, QList<Instance> instances);

    /*!
     * \brief keepBest
     * Keeps the best 'count' members of the population
     * Either by picking all of the best members
     * Or with a slightly more random method
     */
    void keepBest(int count);

    /*!
     * \brief crossover
     * Pairs and crosses members of the population until the population size is count.
     *
     */
    void crossover(int count);

    /*!
     * \brief mutate
     * Mutates the whole population, increasing intensity based on mutation factor
     */
    void mutate(float mutationFactor);

    /*!
     * \brief getBest
     * \return returns the best-scoring member of the population
     */
    CanonicalTDH getBest();

    virtual void onTestStarted(int testN);

    int generation;
    QList<CanonicalTDH> population;
    Map* map;
};

class QTPopulation : public QObject, public Population{
    Q_OBJECT
public:
    QTPopulation(Map* m){
        map = m;
    }

    void onTestStarted(int testN) override;
signals:
    void reportTestStarted(int testN);
};

/*!
 * \brief exportToCSV
 * Exports an entire population to a csv file.
 * Each member of the population is on it's own line.
 * Fields are: index, generation, score, number of nodes, 1x.1y.2x.2y.3x.3y
 */
void exportToCSV(Population population, QString filename);

/*!
 * \brief exportToCSV
 * Exports a single heuristic to a file
 */
void exportToCSV(CanonicalTDH heuristic, QString filename);

/*!
 * \brief importFromCSV
 * Imports heuristics from the CSV file format
 */
QList<CanonicalTDH> importFromCSV(QString filename);

/*!
 * \brief runSimulation
 * Runs a simulation from scratch with the specified parameters
 */
int runSimulation();
int runSimulation(QString mapName, QString instanceName, QString outputName, int pop, int generations, int testCount, int cullRate, float mutation);

/*!
 * \brief The SimulationThread class
 * Runs a simulation within it's own thread.
 * Reports the current generation on the reportProgress.
 */
class SimulationThread : public QThread
{
    Q_OBJECT
    void run() override;
signals:
    void reportProgress(int currentGeneration);
    void reportProgressTest(int testN);
public:
    QString mapName;
    QString instanceName;
    QString outputName;
    int pop;
    int generations;
    int testCount;
    int cullRate;
    float mutation;
};

#endif
