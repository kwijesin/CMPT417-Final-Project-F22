#include "heuristics.h"
#include <cmath>

int Heuristic::getHeuristic(Node N, Node Goal) const
{
    return abs(Goal.x - N.x) + abs(Goal.y - N.y);
}

void CanonicalTDH::setK(int numPivots)
{
    k = numPivots;
}

void CanonicalTDH::calculateTDH()
{
    //TODO
    //Implementation idea
    //for each pivot K, do a breadth first search of the entire space, advancing each search by 1 expansion every loop
    //if you visit a node and it's a pivot node, record the distance between the searcher and that pivot in the primary data
    //if you visit a node and it's unmarked, record the secondary data
    //repeat until all the data is filled out.
    //In the worst case, this takes N*K expansions.

    isCalculated = true;
}

void CanonicalTDH::randomizeNodes(Map map)
{
    nodes.clear();
    while(nodes.length() < k){
        nodes.append(Instance::getRandomValidNode(map));
    }
}

int CanonicalTDH::getHeuristic(Node N, Node Goal) const
{
    Node nearestN = secondary[N.x][N.y].closestPivot;
    Node nearestG = secondary[Goal.x][Goal.y].closestPivot;
    int indNearestN = nodes.indexOf(nearestN);
    int indNearestG = nodes.indexOf(nearestG);
    return primary[indNearestN][indNearestG] - secondary[N.x][N.y].pivotDistance - secondary[Goal.x][Goal.y].pivotDistance;
}

void CanonicalTDH::mutateNodes(Map map, float mutationFactor)
{
    //TODO
}

void AlgorithmicCanonicalTDH::calculateTDH()
{
    //TODO
    //https://webdocs.cs.ualberta.ca/~nathanst/papers/TDH.pdf

    /* 1. choose an un-marked node at random
     * 2. breadth first search of unmarked nodes, marking them as you go, until you mark N/k nodes
     * 3. find the center of the marked patch and place a state there
     * 4. repeat until all nodes are marked
     * 5. sample the set of canonical states until we have exactly k states
     */
}
