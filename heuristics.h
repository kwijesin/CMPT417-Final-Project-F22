#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "instances.h"
#include <QtCore>

/*!
 * \brief The Heuristic class
 *  Base heuristic class
 *  Use for the calculation of the all-pairs data in the canonical TDH
 */
class Heuristic{
	public:
	virtual int getHeuristic(Node N, Node Goal) const;	//TODO base heuristic uses manhattan
};


/*!
 * \brief The CanonicalTDH class
 */
class CanonicalTDH : public Heuristic{
	public:
    CanonicalTDH(){}
    ~CanonicalTDH(){}

    void setK(int numPivots);

    /*!
     * \brief calculateTDH
     *  In order to calculate the memory of the canonical heuristic you need to:
     *      fill out the primary array with all-pairs true distance
     *      fill out the secondary array with the closest pivot and the distance to it
     *
     *  My implementation idea:
     *      for each pivot K, visit all nodes at depth 1
     *          set the secondary data of those nodes to 1,K, unless they have been set by some other pivot
     *      for each pivot K, visit all nodes at depth 2
     *          set the secondary data of those nodes to 2,K, unless they have been set by some other pivot
     *      for each pivot K, visit all nodes at depth 3
     *          set the secondary data of those nodes to 3,K, unless they have been set by some other pivot
     *      etc etc etc
     *      if you visit a node and it's a pivot node, record the distance in the primary data
     *      repeat until the primary data is complete
     */
    void calculateTDH();	//TODO

    /*!
     * \brief randomizeNodes
     *  gets a purely new random set of pivot nodes: use when initializing the first generation
     */
	void randomizeNodes(Map map);	//TODO

    /*!
     * \brief getHeuristic
     *  Reimplementation of inherited function that allows ambiguity in the heuristic that is actually getting used
     *  returns primary(N,Goal) - secondary(N.x, N.y) - secondary(Goal.x, Goal.y)
     */
    int getHeuristic(Node N, Node Goal) const;	//TODO

    /*!
     * \brief mutateNodes
     *  Mutates the nodes according to some mutation factor. The higher the factor, the wilder the mutation
     *  Mutations should move the pivots K around by some gaussian distribution of adjacent nodes
     *  with a small chance to replace one of the pivots with a completely new one
     */
	void mutateNodes(Map map, float mutationFactor);	//TODO
	
    int score;  /*! Contains the last score that this heuristic got. Should be changed by the population evaluation function, therefore this heuristic should be passed by reference */
	int generation;
    QList<Node> nodes;  /*! Ordered list of pivot nodes: no repeats allowed */
	
	private:
	struct SecondaryData{
		int pivotDistance;
		Node closestPivot;
    };
    int k;
    int * primary;	/*! kxk sized dynamic array containing the shortest path distance between node x and node y, ordered by the order in nodes */
    SecondaryData * secondary;	/*! array with same size as the map, contains closest node and the distance to it */
};


/*!
 * \brief The AlgorithmicCanonicalTDH class
 *  This class reimplements calculateTDH in order to use the algorithmic approach outlined in the paper:
 *  https://webdocs.cs.ualberta.ca/~nathanst/papers/TDH.pdf
 */
class AlgorithmicCanonicalTDH : public CanonicalTDH{
    public:
    AlgorithmicCanonicalTDH(){}
    ~AlgorithmicCanonicalTDH(){}

    /*!
     * \brief calculatePDB
     *  Only calculatePDB has to be reimplemented to get the algorithmic approach working
     */
    void calculatePDB();

};

#endif

