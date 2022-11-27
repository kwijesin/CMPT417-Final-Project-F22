#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "instances.h"
#include <QtCore>

class Heuristic{
	public:
	virtual int getHeuristic(Node N, Node Goal) const;	//TODO base heuristic uses manhattan
};

class PDB : public Heuristic{
	public:
	PDB(){}
	~PDB(){}

	void calculatePDB();	//TODO
	void randomizeNodes(Map map);	//TODO
    int getHeuristic(Node N, Node Goal) const;	//TODO
	void mutateNodes(Map map, float mutationFactor);	//TODO
	
	int score;
	int generation;
	QList<Node> nodes;
	
	private:
	struct SecondaryData{
		int pivotDistance;
		Node closestPivot;
    };

	int * primary;	//all pairs shortest path for pivots[P1][P2] (KxK)
	SecondaryData * secondary;	//All nodes closest pivot[a,b] (XxY)
};

#endif

