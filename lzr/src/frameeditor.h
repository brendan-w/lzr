
#pragma once

#include <QtWidgets>
#include <QDebug>

#include "frame.h"
#include "grid.h"
#include "point.h"


/*
 * Editor for Frame models
 * this should really be a subclass of QAbstractItemView,
 * but that was messier than neccessary.
 */
class FrameEditor : public QGraphicsView
{
    Q_OBJECT

public:
    FrameEditor(QWidget* parent = 0);
    ~FrameEditor();

    void setModel(Frame* m);
    void reset();

protected:
    void resizeEvent(QResizeEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void wheelEvent(QWheelEvent* e);
    void drawForeground(QPainter* painter, const QRectF& rect);

private:
    Point* addPoint(lzr::Point orig);
    void resize_graphics();


    Frame* model;
    QGraphicsScene* scene;
    Grid* grid;
    QList<QGraphicsItem*> points;
    QMap<QModelIndex, QList<Point*>> paths;
};
