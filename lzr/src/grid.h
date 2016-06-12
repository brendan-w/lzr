
#pragma once

#include <QtWidgets>
#include <QDebug>



class Grid : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Grid(QGraphicsItem *parent = 0);
    void set_divisions(int d);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget *widget);

private:
    int divisions;
};
