
#include <QtGui>
#include "frameeditor.h"

#define ZOOM_FACTOR 1.2


FrameEditor::FrameEditor(QWidget *parent) : QGraphicsView(parent)
{
    // setRenderHint(QPainter::Antialiasing);
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //enforce custom coordinate system [-1.0, 1.0]
    //Y is negative to make positive values go upwards
    setScene(scene = new QGraphicsScene(this));
    scene->setSceneRect(-5.0, -5.0, 10.0, 10.0); //bigger than (-1.0, -1.0, 2.0, 2.0) for scrollability
    scene->setBackgroundBrush(Qt::black);

    scene->addItem(grid = new Grid);
}

FrameEditor::~FrameEditor()
{

}

void FrameEditor::addPoint()
{
    QGraphicsItem* point = new Point;
    point->setTransform(transform().inverted());
    scene->addItem(point);
    points.append(point);
}

void FrameEditor::resize_graphics()
{
    //when the view is resized, we need update the transform of the individual items
    //to keep them in "pixel" coordinates
    foreach(QGraphicsItem* item, points)
    {
        item->setTransform(transform().inverted());
    }
}

void FrameEditor::resizeEvent(QResizeEvent* e)
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

void FrameEditor::keyPressEvent(QKeyEvent* e)
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

void FrameEditor::keyReleaseEvent(QKeyEvent* e)
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

void FrameEditor::wheelEvent(QWheelEvent* event)
{
    double factor = ZOOM_FACTOR;

    if(event->angleDelta().y() < 0)
        factor = 1.0 / ZOOM_FACTOR;

    scale(factor, factor);
    resize_graphics();
}
