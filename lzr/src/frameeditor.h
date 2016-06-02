
#pragma once

#include <QtWidgets>
#include <QDebug>

#include "grid.h"
#include "point.h"




class FrameEditor : public QGraphicsView
{
    Q_OBJECT

public:
    FrameEditor(QWidget* parent = 0);
    ~FrameEditor();

    void addPoint();
    QSize sizeHint();

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
