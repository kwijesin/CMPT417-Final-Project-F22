#include "instances.h"
#include <QList>
#include <QFile>
#include <QRandomGenerator>

#include <array>

//.scen format indexes
#define IND_BUCKET 0
#define IND_MAP 1
#define IND_MAPW 2
#define IND_MAPH 3
#define IND_STARTX 4
#define IND_STARTY 5
#define IND_GOALX 6
#define IND_GOALY 7
#define IND_LENGTH 8

Map::Map()
{
    xSize = 0;
    ySize = 0;
    N = 0;
}

Map::Map(QString filename)
{
    //https://movingai.com/benchmarks/formats.html
    QFile file(filename);
    QString line;
    const QList<char> passableLand = {'.', 'G', 'S'};
    //const QList<char> impassableLand = {'@', 'O', 'T', 'W'};

    if(!file.exists())
        return;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    //type line
    line = file.readLine();
    //# rows
    line = file.readLine();
    ySize = line.split(' ').at(1).toInt();
    //# columns
    line = file.readLine();
    xSize = line.split(' ').at(1).toInt();
    //map line
    line = file.readLine();

    //initialize map container
    std::vector<bool> defaultColumn;
    defaultColumn.assign(ySize, true);
    map.assign(xSize,defaultColumn);
    N = 0;

    //Origin 0,0 ends up in the top left corner of the map
    for(int y = 0; y < ySize; y++){
        QByteArray mapRow = file.readLine();
        for(int x = 0; x < xSize; x++){
            if(passableLand.contains(mapRow[x]))
                N++;
            else
                map[x][y] = false;
        }
    }

    file.close();
}

QList<Instance> Instance::importInstances(QString filename)
{
    //https://movingai.com/benchmarks/formats.html
    QList<Instance> ret;

    QFile file(filename);
    QString line;
    QStringList splitLine;
    Node start, goal;

    if(!file.exists())
        return ret;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return ret;

    //Version line
    line = file.readLine();
    //Instances
    while(!file.atEnd()){
        line = file.readLine();
        splitLine = line.split('\t');
        start.x = splitLine.at(IND_STARTX).toInt();
        start.y = splitLine.at(IND_STARTY).toInt();
        goal.x = splitLine.at(IND_GOALX).toInt();
        goal.y = splitLine.at(IND_GOALY).toInt();
        ret.append(Instance(start, goal));
    }

    file.close();

    return ret;
}

QList<Instance> Instance::getRandomTestSuite(QList<Instance> allInstances, int count)
{
    QList<Instance> ret;
    while(ret.length() < count)
        ret.append(allInstances.takeAt(QRandomGenerator::global()->bounded(0,allInstances.count()-1)));
    return ret;
}

Node getRandomValidNode(Map map){
    Node ret;
    int x,y;
    QRandomGenerator* gen = QRandomGenerator::global();
    do{
        x = gen->bounded(0, map.xSize-1);
        y = gen->bounded(0, map.ySize-1);
    }while(!map.isOpen(x, y));
    ret.x = x;
    ret.y = y;
    return ret;
}

QList<Instance> Instance::generateRandomTestSuite(Map map, int count)
{
    QList<Instance> ret;
    while(ret.length() < count){
        Instance add(getRandomValidNode(map), getRandomValidNode(map));
        ret.append(add);
    }
    return ret;
}
