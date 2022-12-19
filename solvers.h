#ifndef SOLVERS_H
#define SOLVERS_H

#include "instances.h"
#include "heuristics.h"

#include <QMap>
#include <set>

/*!
 * \brief The NodeHeap class
 * Provides a sorted list of nodes using std::multiset. Used for solvers.
 */
class NodeHeap : public std::multiset<Node>{
public:
    void push(Node node){
        insert(node);
    }
    Node pop(){
        return extract(begin()).value();
    }
};

/*!
 * \brief The Solver class
 *  Generic solver class that should use BFS: used when we calculate canonical TDH?
 */
class Solver{
	public:
	Solver(){}
	~Solver(){}

    virtual bool solve(Map map, Instance instance, int* pathLength, int* nExpansions, Heuristic* heuristic = NULL) const;
    virtual bool solve(Map map, Instance instance, QList<Node>* path, int* nExpansions, Heuristic* heuristic = NULL) const;

};

/*!
 * \brief The AStar class
 *  A* solver, what more to say
 */
class AStar : public Solver{
	public:
	AStar(){}
	~AStar(){}

    bool solve(Map map, Instance instance, QList<Node>* path, int* nExpansions, Heuristic* heuristic = NULL) const override;

};

/*!
 * \brief The IDAStar class
 *  Optional setup headers for an IDA* solver in case we want to benchmark it against normal A* when using it for genetic testing
 */
class IDAStar : public Solver{
	public:
	IDAStar(){}
	~IDAStar(){}

    bool solve(Map map, Instance instance, QList<Node>* path, int* nExpansions, Heuristic* heuristic = NULL) const override;
};

#endif
