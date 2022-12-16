#include "heuristics.h"
#include "solvers.h"
#include <cmath>
#include <random>
#include <set>

int Heuristic::getHeuristic(Node N, Node Goal) const
{
    return abs(Goal.x - N.x) + abs(Goal.y - N.y);
}

void CanonicalTDH::setK(int numPivots)
{
    isCalculated = false;
    k = numPivots;
}


//returns list of valid nodes adjacent to 'expanded'
QList<Node> expandNode(Node expanded, Map map){
    QList<Node> ret;
    int dirX[] = {-1, -1, 1, 1};
    int dirY[] = {-1, 1, -1, 1};
    for(int i = 0; i < 3; i++){
        Node newNode = expanded;
        newNode.x += dirX[i];
        newNode.y += dirY[i];
        if(map.isOpen(newNode))
            ret.append(newNode);
    }
    return ret;
}

void CanonicalTDH::calculateTDH(Map map)
{
    //initialize TDH arrays
    primary.clear();
    std::vector<int> copyP;
    copyP.assign(k,-1);
    primary.assign(k, copyP);
    secondary.clear();
    std::vector<SecondaryData> copyS;
    copyS.assign(map.ySize, SecondaryData());
    secondary.assign(map.xSize, copyS);

    //initialize calculation variables
    QList<NodeHeap> openLists; //NodeHeap is sorted
    int numPrimaryFilled = 0;   //up to k*k
    int numSecondaryFilled = 0; //up to map.N
    int depth = 0;

    //initialize open lists
    for(int i = 0; i < nodes.length(); i++){
        Node pivot = nodes.at(i);
        pivot.g = 0;
        pivot.parentx = pivot.x;
        pivot.parenty = pivot.y;
        openLists[i].push(pivot);
    }

    //run K concurrent depth first searches, advancing 1 depth at a time
    while(numPrimaryFilled < (k*k) || numSecondaryFilled < map.N){
        for(int i = 0; i < k; i++){
            Node currentPivot = nodes.at(i);
            int currentPivotInd = nodes.indexOf(currentPivot);
            NodeHeap * currentOpenList = &openLists.data()[currentPivotInd];    //access the data of the openList directly

            if(currentOpenList->empty()){
                continue;
            }

            //expand everything at the current depth
            while(!currentOpenList->empty() && currentOpenList->begin()->f() <= depth){
                Node expandingNode = currentOpenList->pop();
                QList<Node> expandingNodes = expandNode(expandingNode, map);

                //if the node is a pivot k and hasn't been filled in yet, fill in the primary array
                if(nodes.contains(expandingNode)){
                    int encounteredPivotInd = nodes.indexOf(expandingNode);
                    if(primary[currentPivotInd][encounteredPivotInd] != -1){
                        primary[currentPivotInd][encounteredPivotInd] = expandingNode.g;
                        numPrimaryFilled++;
                    }
                    if(primary[encounteredPivotInd][currentPivotInd] != -1){
                        primary[encounteredPivotInd][currentPivotInd] = expandingNode.g;
                        numPrimaryFilled++;
                    }
                }

                //if the node is un-filled, fill it
                if(secondary[expandingNode.x][expandingNode.y].pivotDistance == -1){
                    secondary[expandingNode.x][expandingNode.y].closestPivot = currentPivot;
                    secondary[expandingNode.x][expandingNode.y].pivotDistance = expandingNode.g;
                    numSecondaryFilled++;
                }

                //expand the node
                for(int j = 0; i < expandingNodes.length(); j++){
                    Node childNode = expandingNodes.at(j);
                    childNode.g = expandingNode.g + 1;
                    childNode.parentx = expandingNode.x;
                    childNode.parenty = expandingNode.y;
                    currentOpenList->push(childNode);
                }
            }
        }
        depth++;
    }

    isCalculated = true;
}

void CanonicalTDH::randomizeNodes(Map map)
{
    isCalculated = false;
    nodes.clear();
    while(nodes.length() < k){
        nodes.append(map.getRandomValidNode());
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
    isCalculated = false;

    std::mt19937 stdGen(QRandomGenerator::global()->generate());
    std::normal_distribution<float> gNorm(0, mutationFactor);

    //moves each node by a normal distribution
    QList<Node> newList;
    nodes.clear();
    for(int i = 0; i < k; i++){
        Node newNode;
        int xOff,yOff;
        bool valid = false;
        while(!valid){
            xOff = qRound(gNorm(stdGen));
            yOff = qRound(gNorm(stdGen));
            newNode = nodes.at(i);
            newNode.x += xOff;
            newNode.y += yOff;
            if(map.isOpen(newNode) && !newList.contains(newNode))
                valid = true;
        }
        newList.append(newNode);
    }
    nodes = newList;
}

void AlgorithmicCanonicalTDH::calculateTDH(Map map)
{
    //TODO
    //https://webdocs.cs.ualberta.ca/~nathanst/papers/TDH.pdf

    /* 1. choose an un-marked node at random
     * 2. breadth first search of unmarked nodes, marking them as you go, until you mark N/k nodes
     * 3. find the center of the marked patch and place a state there
     * 4. repeat until all nodes are marked
     * 5. sample the set of canonical states until we have exactly k states
     */
    isCalculated = true;
}
