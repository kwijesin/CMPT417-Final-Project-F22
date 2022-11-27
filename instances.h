#ifndef INSTANCES_H
#define INSTANCES_H

#include <QString>

struct Node{
	int x;
	int y;
};

class Map{
	public:
    Map(){} //TODO
	Map(QString filename){}	//TODO
	~Map(){
		delete map;
	}
	bool isOpen (Node N) const{
		return isOpen(N.x, N.y);
	}
    bool isOpen(int x, int y) const{
		if(x <= xSize && y < ySize){
            return map[x][y];
		}
		return false;
	}
	int numOpenNodes();	//TODO

	static Map importMap(QString filename){
		return Map(filename);
	}

	int xSize;
	int ySize;
	int N;
    bool** map;
};

class Instance{
	public:
	Instance(Node Start, Node Goal){
		start = Start;
        goal = Goal;
	}
	~Instance(){}

	static QList<Instance> importInstances(QString filename);	//TODO
	static QList<Instance> getRandomTestSuite(QList<Instance> allInstances, int count);	//TODO
    static QList<Instance> generateRandomTestSuite(Map map, int count);	//TODO
	
	Node start;
	Node goal;
};

#endif
