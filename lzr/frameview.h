
#pragma once

#include <QtWidgets>
#include <QDebug>

#include "grid.h"
#include "point.h"




class FrameView : public QGraphicsView
{
    Q_OBJECT

public:
    FrameView(QWidget* parent = 0);
    ~FrameView();

    void addPoint();
    QSize sizeHint();

    void fillWindow();

protected:
    void resizeEvent(QResizeEvent* e);
    void wheelEvent(QWheelEvent* event);

private:
    void addItem(QGraphicsItem* item);

    QGraphicsScene* scene;
    Grid* grid;
};
