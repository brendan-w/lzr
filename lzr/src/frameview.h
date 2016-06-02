
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

    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

    void wheelEvent(QWheelEvent* e);

private:
    void addItem(QGraphicsItem* item);
    void resize_graphics();

    QGraphicsScene* scene;
    Grid* grid;
    QList<QGraphicsItem*> points;
};
