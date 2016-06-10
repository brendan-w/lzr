
#pragma once

#include <QtWidgets>
#include <QDebug>

#include "frame.h"
#include "grid.h"
#include "point.h"



class Path : public QGraphicsObject
{
    Q_OBJECT

public:
    Path(lzr::Frame frame);
    QRectF boundingRect() const;

public slots:
    void point_changed();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};



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

private:
    void resize_graphics();

    Frame* model;
    QGraphicsScene* scene;
    Grid* grid;
    QList<QGraphicsObject*> points;
    QList<Path*> paths;
};
