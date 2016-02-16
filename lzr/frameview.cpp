
#include <QtGui>
#include "frameview.h"
#include "point.h"


FrameView::FrameView(QWidget *parent) : QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing);

    //enforce custom coordinate system [-1.0, 1.0]
    //Y is negative to make positive values go upwards

    scene = new QGraphicsScene(this);
    scene->setSceneRect(-1.0, -1.0, 2.0, 2.0);
    scene->setBackgroundBrush(Qt::black);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(scene);
}

FrameView::~FrameView()
{

}

void FrameView::addPoint()
{
    QGraphicsItem* point = new Point;
    point->setTransform(transform().inverted());
    scene->addItem(point);
}

void FrameView::resizeEvent(QResizeEvent* e)
{
    //resize
    QGraphicsView::resizeEvent(e);

    //make a new transformation matrix that's scaled correctly
    QTransform t;
    double s = qMin(width(), height()) / scene->sceneRect().width(); // min(w, h) / 2.0
    t.scale(s, -s);
    setTransform(t);

    //when the view is resized, we need update the transform of the individual items
    //to keep them in "pixel" coordinates
    foreach(QGraphicsItem* item, items())
    {
        item->setTransform(t.inverted());
    }
}
