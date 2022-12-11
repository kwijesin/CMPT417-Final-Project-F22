#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include "instances.h"
#include "heuristics.h"
#include <QLabel>
#include <QPixmap>
#include <QPainter>

//Graphics widget that can be used to show a map

class MapWidget : public QLabel
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr) :
        QLabel{parent}
    {

    }

    void setMap(Map m)
    {
        map = m;
        hasMap = true;
        redraw();
    }

    void setMap(QString filename)
    {
        if(QFile::exists(filename))
            setMap(Map(filename));
    }

    void setHeuristic(CanonicalTDH h){
        heuristic = h;
        hasHeuristic = true;
        redraw();
    }

    void redraw(){
        QPixmap pixMap(map.xSize, map.ySize);
        pixMap.fill(Qt::black);
        QPainter pixPainter(&pixMap);

        //draw map
        for(int y = 0; y < map.ySize; y++){
            for(int x = 0; x < map.ySize; x++){
                pixPainter.setPen(map.map[x][y] ? Qt::white :Qt::black);
                pixPainter.drawPoint(x,y);
            }
        }

        //draw circles around each point K if there's a heuristic
        if(hasHeuristic){
            pixPainter.setPen(Qt::red);
            for(int i = 0; i < heuristic.nodes.length(); i++){
                Node toDraw = heuristic.nodes.at(i);
                pixPainter.drawEllipse(toDraw.x, toDraw.y, 2, 2);
            }
        }

        //Save image
        //QFile file("repaintedmap.png");
        //file.open(QIODevice::WriteOnly);
        //pixMap.save(&file, "PNG");
        //file.close();

        //scale and paint the map onto this widget
        QPixmap scaledPixMap = pixMap.scaled(size(),Qt::KeepAspectRatio);
        this->setPixmap(scaledPixMap);
    }

signals:

private:
    bool hasMap = false;
    Map map;

    bool hasHeuristic = false;
    CanonicalTDH heuristic;
};

#endif // MAPWIDGET_H
