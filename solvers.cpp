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
    //TODO
    return false;
}

bool AStar::solve(Map map, Instance instance, QList<Node>* path, Heuristic* heuristic) const
{
    //TODO
    return false;
}

bool IDAStar::solve(Map map, Instance instance, QList<Node>* path, Heuristic* heuristic) const
{
    //TODO
    return false;
}
