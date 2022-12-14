#ifndef INSTANCES_H
#define INSTANCES_H

#include <QString>
#include <QList>

struct Node{
    //normal nodes
	int x;
    int y;

    //used for pathfinding
    int h;
    int g;
    int f() const{
        return h+g;
    }
    int parentx;
    int parenty;

    bool operator < (Node a) const{
        return f() < a.f();
    }

    bool operator == (Node a) const{
        return x == a.x && y == a.y;
    }
};

typedef QList<Node> Path;

/*!
 * Opens .txt files from the MAPF benchmarks website:
 * https://movingai.com/benchmarks/mapf.html
 */
class Map{
    public:
    Map();
    Map(QString filename);
    ~Map(){}

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
    std::vector<std::vector<bool>> map; // double vector, set capacity to xSize ySize using reserve
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

    static Node getRandomValidNode(Map map);

    /*!
     * \brief importInstances
     *  Returns a list of instances gotten from the instance filetypes from the benchmark website
     * \param filename: filetype from https://movingai.com/benchmarks/mapf.html
     */
    static QList<Instance> importInstances(QString filename);

    /*!
     * \brief getRandomTestSuite
     *  Returns a random subset of allInstances
     */
    static QList<Instance> getRandomTestSuite(QList<Instance> allInstances, int count);

    /*!
     * \brief generateRandomTestSuite:
     *  Generates a new random set of instances for the map instead of getting them from a file
     */
    static QList<Instance> generateRandomTestSuite(Map map, int count);
	
	Node start;
	Node goal;
};

#endif
