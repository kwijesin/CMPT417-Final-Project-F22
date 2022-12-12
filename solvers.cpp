#include "solvers.h"

bool Solver::solve(Map map, Instance instance, int* pathLength, Heuristic* heuristic) const
{
    QList<Node> path;
    bool ret = solve(map, instance, &path, heuristic);
    *pathLength = path.length();
    return ret;
}
bool Solver::solve(Map map, Instance instance, QList<Node>* path, Heuristic* heuristic) const
{
    //TODO: BFS
    return false;
}

bool AStar::solve(Map map, Instance instance, QList<Node>* path, Heuristic* heuristic) const
{
    //TODO: AStar
    //Example code for how to use the class NodeHeap

    NodeHeap openList;
    Node pushNode;
    openList.push(pushNode);
    Node poppedNode = openList.pop();

    return false;
}

bool IDAStar::solve(Map map, Instance instance, QList<Node>* path, Heuristic* heuristic) const
{
    //TODO: IDAStar
    return false;
}
