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
        hasHeuristic = false;
        redraw();
    }

    void setMap(QString filename)
    {
        if(QFile::exists(filename))
            setMap(Map(filename));
    }

    void setHeuristic(Map m, CanonicalTDH h){
        map = m;
        hasMap = true;
        heuristic = h;
        hasHeuristic = true;
        redraw();
    }

    void redraw(){
        QPixmap pixMap(map.xSize, map.ySize);
        pixMap.fill(Qt::black);
        QPainter pixPainter(&pixMap);

        if(hasHeuristic){
            //choose a random light blue-greenish color for each pivot K
            QList<QColor> colors;
            for(int i = 0; i < heuristic.k; i++){
                int red = QRandomGenerator::global()->bounded(0,200);
                int green = QRandomGenerator::global()->bounded(100,255);
                int blue = QRandomGenerator::global()->bounded(100,255);
                QColor color = QColor::fromRgb(red,green,blue);
                colors.append(color);
            }

            //paint each pixel the color of the nearest pivot
            for(int y = 0; y < map.ySize; y++){
                for(int x = 0; x < map.xSize; x++){
                    Node nearestPivot = heuristic.secondary[x][y].closestPivot;
                    int pivotIndex = heuristic.nodes.indexOf(nearestPivot);
                    pixPainter.setPen(map.isOpen(x,y) ? colors.at(pivotIndex) : Qt::black);
                    pixPainter.drawPoint(x,y);
                }
            }

            //draw red on each pivot K
            pixPainter.setPen(Qt::red);
            for(int i = 0; i < heuristic.nodes.length(); i++){
                Node toDraw = heuristic.nodes.at(i);
                //pixPainter.drawEllipse(toDraw.x, toDraw.y, 2, 2);
                pixPainter.drawPoint(toDraw.x, toDraw.y);
            }
        }else{
            //draw map normally
            for(int y = 0; y < map.ySize; y++){
                for(int x = 0; x < map.ySize; x++){
                    pixPainter.setPen(map.isOpen(x,y) ? Qt::white : Qt::black);
                    pixPainter.drawPoint(x,y);
                }
            }
        }

        //scale and paint the map onto this widget
        QPixmap scaledPixMap = pixMap.scaled(size(),Qt::KeepAspectRatio);
        this->setPixmap(scaledPixMap);
    }

    void saveImage(QString filename){
        QPixmap myMap = this->pixmap();
        QFile file(filename);
        file.open(QIODevice::WriteOnly);
        myMap.save(&file, "PNG");
        file.close();
    }

signals:

private:
    bool hasMap = false;
    Map map;

    bool hasHeuristic = false;
    CanonicalTDH heuristic;
};

#endif // MAPWIDGET_H
