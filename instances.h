#ifndef INSTANCES_H
#define INSTANCES_H

#include <QString>

struct Node{
	int x;
	int y;
};

/*!
 * Opens .txt files from the MAPF benchmarks website:
 * https://movingai.com/benchmarks/mapf.html
 */
class Map{
    public:
    Map(QString filename){}
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

    int numOpenNodes(){
        return N;
    }

	static Map importMap(QString filename){
		return Map(filename);
	}

	int xSize;
	int ySize;
	int N;
    bool** map; /*! Double pointer to a dynamically allocated array indexed by x,y position, row major. Use New and Delete */
};

/*!
 * \brief The Instance class
 * Contains a start location and goal location
 * Used for other classes
 */
class Instance{
    public:
	Instance(Node Start, Node Goal){
		start = Start;
        goal = Goal;
	}
	~Instance(){}

    /*!
     * \brief importInstances
     *  Returns a list of instances gotten from the instance filetypes from the benchmark website
     * \param filename: filetype from https://movingai.com/benchmarks/mapf.html
     */
	static QList<Instance> importInstances(QString filename);	//TODO

    /*!
     * \brief getRandomTestSuite
     *  Returns a random subset of allInstances
     */
	static QList<Instance> getRandomTestSuite(QList<Instance> allInstances, int count);	//TODO

    /*!
     * \brief generateRandomTestSuite:
     *  Generates a new random set of instances for the map instead of getting them from a file
     */
    static QList<Instance> generateRandomTestSuite(Map map, int count);	//TODO
	
	Node start;
	Node goal;
};

#endif
