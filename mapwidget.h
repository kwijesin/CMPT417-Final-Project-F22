#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include "instances.h"
#include <QWidget>

//Graphics widget that can be used to show a map

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr) :
        QWidget{parent}
    {

    }

    void setMap(Map map);
    void setMap(QString filename);
    void setInstance(Instance instance);

    void paintEvent(QPaintEvent *event)
    {

    }

signals:

private:
    Map map;

};

#endif // MAPWIDGET_H
