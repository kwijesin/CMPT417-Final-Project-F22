#ifndef SOLVERS_H
#define SOLVERS_H

#include "instances.h"
#include "heuristics.h"

/*!
 * \brief The Solver class
 *  Generic solver class that should use dijkstra's algorithm or something, maybe we don't implement it at all
 */
class Solver{
	public:
	Solver(){}
	~Solver(){}

    virtual bool solve(Map map, Instance instance, int* pathLength, Heuristic* heuristic = NULL) const;
    virtual bool solve(Map map, Instance instance, QList<Node>* path, Heuristic* heuristic = NULL) const;

};

/*!
 * \brief The AStar class
 *  A* solver, what more to say
 */
class AStar : public Solver{
	public:
	AStar(){}
	~AStar(){}

    bool solve(Map map, Instance instance, QList<Node>* path, Heuristic* heuristic = NULL) const;

};

/*!
 * \brief The IDAStar class
 *  Optional setup headers for an IDA* solver in case we want to benchmark it against normal A* when using it for genetic testing
 */
class IDAStar : public Solver{
	public:
	IDAStar(){}
	~IDAStar(){}

    bool solve(Map map, Instance instance, QList<Node>* path, Heuristic* heuristic = NULL) const;
};

#endif
