
#pragma once

#include <QtWidgets>
#include <QDebug>



class Grid : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Grid(QGraphicsItem *parent = 0);
    ~Grid();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    size_t divisions;
    QPen pen;
};
