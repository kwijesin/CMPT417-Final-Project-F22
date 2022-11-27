#ifndef SOLVERS_H
#define SOLVERS_H

#include "instances.h"
#include "heuristics.h"

class Solver{
	public:
	Solver(){}
	~Solver(){}

	virtual bool solve(Map map, Instance instance, Heuristic* heuristic = NULL) const;

};

class AStar : public Solver{
	public:
	AStar(){}
	~AStar(){}

	bool solve(Map map, Instance instance, Heuristic* heuristic = NULL) const;	//TODO

};

//Optional
class IDAStar : public Solver{
	public:
	IDAStar(){}
	~IDAStar(){}

	bool solve(Map map, Instance instance, Heuristic* heuristic = NULL) const;
};

#endif
