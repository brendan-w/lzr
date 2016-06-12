
#pragma once

#include <QtWidgets>
#include <QDebug>



class Grid : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Grid(QGraphicsItem *parent = 0);
    void set_divisions(int d);
    void set_snapping(bool snap);
    QPointF constrain_and_maybe_snap(QPointF p);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget);

private:
    int divisions;
    bool snapping;

    QPointF constrain_to_frame(QPointF p);
    QPointF snap_to_grid(const QPointF& point);
    float single_axis_snap(float x);
};
