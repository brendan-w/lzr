
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

void FrameEditor::reset()
{
    foreach(const QList<Point*>& points, paths)
    {
        foreach(Point* point, points)
        {
            scene->removeItem(point);
            delete point;
        }
    }

    paths.clear();
    //TODO: disconnect signals from the model
    model = NULL;
}

void FrameEditor::setModel(Frame* m)
{
    reset();
    model = m;
    //TODO connect signals

    //create all of the path objects in the model
    for(int i = 0; i < model->rowCount(); i++)
    {
        QModelIndex index = model->index(i);
        lzr::Frame path = index.data().value<lzr::Frame>();

        foreach(const lzr::Point& p, path)
        {
            paths[index].append(addPoint(p));
        }
    }
}


Point* FrameEditor::addPoint(lzr::Point orig)
{
    Point* point = new Point(orig);
    point->setTransform(transform().inverted());
    scene->addItem(point);
    return point;
}

void FrameEditor::drawForeground(QPainter* painter, const QRectF& rect)
{
    //draw the lines between points
    /*
    for(int i = 0; i < points.size() - 1; i++)
    {
        Point& p1 = points[i];
        Point& p2 = points[i+1];
        painter->setPen(QPen(QColor(p2.r, p2.g, p2.b), 0));
        painter->drawLine(p1.x(), p1.y(), p2.x(), p2.y());
    }
    */
}


void FrameEditor::resize_graphics()
{
    //when the view is resized, we need update the transform of the individual items
    //to keep them in "pixel" coordinates
    foreach(const QList<Point*>& points, paths)
    {
        foreach(Point* point, points)
        {
            point->setTransform(transform().inverted());
        }
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
