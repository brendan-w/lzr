
#pragma once

#include <QtWidgets>
#include <QDebug>



class Grid : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Grid(QGraphicsItem *parent = 0);
    void set_divisions(int d);
    QPointF snap_to_grid(const QPointF& point);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget);

private:
    int divisions;

    float single_axis_snap(float x);
};
