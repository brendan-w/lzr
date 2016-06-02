
#pragma once

#include <QtWidgets>
#include <QDebug>



class Grid : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    Grid(QGraphicsItem *parent = 0);
    ~Grid();

public slots:
    void setDivisions(size_t d);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    size_t divisions;
};
