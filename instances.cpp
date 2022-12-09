#include "instances.h"
#include <QList>
#include <QFile>

#include <array>

Map::Map()
{
    //TODO
}

Map::Map(QString filename)
{
    QFile file(filename);
    QString line;

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
            if(mapRow[x] == '.')
                N++;
            else
                map[x][y] = false;
        }
    }

    file.close();
}

QList<Instance> Instance::importInstances(QString filename)
{
    //TODO
    QList<Instance> ret;
    return ret;
}

QList<Instance> Instance::getRandomTestSuite(QList<Instance> allInstances, int count)
{
    //TODO
    QList<Instance> ret;
    return ret;
}

QList<Instance> Instance::generateRandomTestSuite(Map map, int count)
{
    //TODO
    QList<Instance> ret;
    return ret;
}
