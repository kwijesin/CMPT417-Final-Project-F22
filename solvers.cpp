#include "solvers.h"
#include <unordered_set>
#include <set>
#include <map>
#include <vector>

bool Solver::solve(Map map, Instance instance, int* pathLength, int* nExpansions, Heuristic* heuristic) const
{
    QList<Node> path;
    bool ret = solve(map, instance, &path, nExpansions, heuristic);
    *pathLength = path.length();
    return ret;
}

bool Solver::solve(Map map, Instance instance, QList<Node>* path, int* nExpansions, Heuristic* heuristic) const
{
    int numExpansions = 0;
    NodeHeap openList;
    std::set<Node> visitedNodes;
    bool goalFound = false;
    openList.push(instance.start);
    visitedNodes.insert(instance.start);
    while (!openList.empty() && !goalFound){
        Node expandingNode = openList.pop();
        numExpansions++;
        if(expandingNode == instance.goal){
            goalFound = true;
            break;
        }
        QList<Node> adjacent = map.adjacentNodes(expandingNode);
        for(int i = 0; i < adjacent.length(); i++){
            Node child = adjacent.at(i);
            if(visitedNodes.find(child) == visitedNodes.end()){
                visitedNodes.insert(child);
                openList.push(child);
            }
        }
    }
    if(goalFound){
        *nExpansions += numExpansions;
    }
    return goalFound;
}

bool AStar::solve(Map map, Instance instance, QList<Node>* path, int* nExpansions, Heuristic* heuristic) const
{
    int numExpansions = 0;
    bool goalFound = false;
    NodeHeap openList;
    QMap<Node, int> visitedNodesGScore;
    openList.push(instance.start);
    visitedNodesGScore.insert(instance.start, 0);
    while(!openList.empty()){
        Node currentNode = openList.pop();
        numExpansions++;
        if(currentNode == instance.goal){
            goalFound = true;
            break;
        }
        QList<Node> neighbours = map.adjacentNodes(currentNode);
        for(int i = 0; i < neighbours.length(); i++){
            Node child = neighbours.at(i);
            child.g = currentNode.g + 1;
            child.h = heuristic->getHeuristic(child, instance.goal);

            if(!visitedNodesGScore.contains(child)){
                visitedNodesGScore.insert(child, child.g);
                openList.push(child);
            }else{
                int oldGScore = visitedNodesGScore.value(child);
                if(oldGScore > child.g){ //New best path to this node found
                    visitedNodesGScore.insert(child, child.g);
                    if(openList.find(child) != openList.end()){
                        openList.erase(child);
                    }
                    openList.push(child);
                }
            }
        }
    }
    if(goalFound){
        *nExpansions += numExpansions;
    }
    return goalFound;
}

bool IDAStar::solve(Map map, Instance instance, QList<Node>* path, int* nExpansions, Heuristic* heuristic) const
{
    return false;
}
