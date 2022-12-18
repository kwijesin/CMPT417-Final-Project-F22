#include "heuristics.h"
#include "solvers.h"
#include <cmath>
#include <random>
#include <set>

int Heuristic::getHeuristic(Node N, Node Goal) const
{
    return abs(Goal.x - N.x) + abs(Goal.y - N.y);
}

CanonicalTDH::CanonicalTDH(int gen)
{
    generation = gen;
}

void CanonicalTDH::setK(int numPivots)
{
    isCalculated = false;
    k = numPivots;
}

void CanonicalTDH::calculateTDH(Map map)
{
    //Assumes that K and nodes are initialized by some other means

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
                QList<Node> expandingNodes = map.adjacentNodes(expandingNode);

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

struct coordinate{
    int x;
    int y;
};

QSet<coordinate> getUnmarkedList(Map map){
    QSet<coordinate> ret;
    for(int y = 0; y < map.ySize; y++){
        for(int x = 0; x < map.ySize; x++){
            if(map.map[x][y]){
                coordinate add;
                add.x = x;
                add.y = y;
                ret.insert(add);
            }
        }
    }
    return ret;
}

template <typename K> K randomKeyFromHash(QSet<K> list){
    int chosenInd = QRandomGenerator::global()->bounded(list.count());
    QSetIterator<K> itr(list);
    for(int i = 0; i < chosenInd-1; i++)
        itr.next();
    return itr.next();
}

void AlgorithmicCanonicalTDH::calculateTDH(Map map)
{
    //https://webdocs.cs.ualberta.ca/~nathanst/papers/TDH.pdf
    /* 1. choose an un-marked node at random to be a new pivot
     * 2. breadth first search of unmarked nodes, marking them as you go, until you mark N/k nodes
     * 4. repeat until all nodes are marked
     * 5. repeat the whole process until we're within 10% of the intended number of k
     */

    //assumes that k is initialized
    //may change K by a small amount
    //creates nodes from scratch

    const int areaOfZone = map.N/k;
    bool validTo10Percent = false;
    int actualK = 0;
    while(!validTo10Percent){
        nodes.clear();
        QSet<coordinate> markedList;
        QSet<coordinate> unmarkedList = getUnmarkedList(map);
        actualK = 0;

        NodeHeap openList;
        int numNodesMarked = 0;   //up to k*k

        while(numNodesMarked < map.N){
            int numNodesInZone = 1;

            //find a random unmarked point to use as a new pivot
            coordinate root = randomKeyFromHash(unmarkedList);
            Node newPivot;
            newPivot.x = root.x;
            newPivot.y = root.y;
            newPivot.g = 0;
            nodes.append(newPivot);
            markedList.insert(root);
            unmarkedList.remove(root);
            actualK++;

            //DFS areaOfZone points branching from that point
            NodeHeap openList;
            newPivot.g = 0;
            openList.push(newPivot);
            while(numNodesInZone < areaOfZone && !openList.empty()){
                Node expandingNode = openList.pop();
                QList<Node> childNodes = map.adjacentNodes(expandingNode);
                for(int i = 0; i < childNodes.length(); i++){
                    Node childNode = childNodes.at(i);
                    coordinate child;
                    child.x = childNode.x;
                    child.y = childNode.y;
                    if(unmarkedList.contains(child)){
                        unmarkedList.remove(child);
                        markedList.insert(child);
                        childNode.g = expandingNode.g + 1;
                        numNodesInZone++;
                    }
                }
            }
        }

        //check if we got within 10% k
        if(abs(actualK - k) < float(k)/10)
            validTo10Percent = true;
    }

    k = actualK;

    CanonicalTDH::calculateTDH(map);

    isCalculated = true;
}
