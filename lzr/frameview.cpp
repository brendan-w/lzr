
#include <QtGui>
#include "frameview.h"


FrameView::FrameView(QWidget *parent) : QGraphicsView(parent)
{
    // setRenderHint(QPainter::Antialiasing);

    //enforce custom coordinate system [-1.0, 1.0]
    //Y is negative to make positive values go upwards

    scene = new QGraphicsScene(this);
    // scene->setSceneRect(-1.0, -1.0, 2.0, 2.0);
    scene->setSceneRect(-5.0, -5.0, 10.0, 10.0);
    scene->setBackgroundBrush(Qt::black);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(scene);

    grid = new Grid;
    scene->addItem(grid);
}

FrameView::~FrameView()
{

}

void FrameView::addPoint()
{
    QGraphicsItem* point = new Point;
    point->setTransform(transform().inverted());
    scene->addItem(point);
    points.append(point);
}

void FrameView::resize_graphics()
{
    //when the view is resized, we need update the transform of the individual items
    //to keep them in "pixel" coordinates
    foreach(QGraphicsItem* item, points)
    {
        item->setTransform(transform().inverted());
    }
}

void FrameView::resizeEvent(QResizeEvent* e)
{
    //resize
    QGraphicsView::resizeEvent(e);

    //make a new transformation matrix that's scaled correctly
    QTransform t;
    double s = qMin(width(), height()) / scene->itemsBoundingRect().width(); // min(w, h) / 2.0
    t.scale(s, -s);
    setTransform(t);

    resize_graphics();
}

void FrameView::keyPressEvent(QKeyEvent* e)
{
    if(!e->isAutoRepeat())
    {
        if(e->key() == Qt::Key_Space)
        {
            setInteractive(false);
            setDragMode(ScrollHandDrag);
        }
    }

    QGraphicsView::keyPressEvent(e);
}

void FrameView::keyReleaseEvent(QKeyEvent* e)
{
    if(!e->isAutoRepeat())
    {
        if(e->key() == Qt::Key_Space)
        {
            setInteractive(true);
            setDragMode(NoDrag);
        }
    }

    QGraphicsView::keyReleaseEvent(e);
}

void FrameView::wheelEvent(QWheelEvent* event)
{
    double factor = 1.2;
    if(event->angleDelta().y() < 0)
        factor = 1.0 / factor;

    scale(factor, factor);
    resize_graphics();
}
