
#include <QtGui>
#include "frameeditor.h"

#define ZOOM_FACTOR 1.2


FrameEditor::FrameEditor(QWidget *parent) : QGraphicsView(parent)
{
    // setRenderHint(QPainter::Antialiasing);
    setFrameStyle(QFrame::NoFrame);
    //disable all scroll bars
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //when points move, the connecting lines must be also redrawn
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

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
    foreach(Path* path, paths)
    {
        scene->removeItem(path);
        delete path;
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
        lzr::Frame lzr_path = index.data().value<lzr::Frame>();

        Path* path = new Path(lzr_path);
        paths.append(path);
        scene->addItem(path);
    }
}

void FrameEditor::resize_graphics()
{
    //when the view is resized, we need update the transform of the individual items
    //to keep them in "pixel" coordinates

    foreach(Path* path, paths)
    {
        //path->setTransform(transform().inverted());
        foreach(QGraphicsItem* point, path->childItems())
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





Path::Path(lzr::Frame frame)
{
    for(lzr::Point lzr_point : frame)
    {
        Point* point = new Point(lzr_point);
        point->setParentItem(this);
    }
}

QRectF Path::boundingRect() const
{
    return QRectF(-1,-1,2,2); //the whole screen/grid
}

void Path::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<QGraphicsItem*> points = childItems();
    for(int i = 0; i < points.size() - 1; i++)
    {
        const Point* p1 = (Point*) points[i];
        const Point* p2 = (Point*) points[i+1];
        QLineF line(p1->x(), p1->y(), p2->x(), p2->y());
        painter->setPen(QPen(p2->getColor(), 0));
        painter->drawLine(line);
    }
}

QVariant Path::itemChange(GraphicsItemChange change, const QVariant &value)
{
    qDebug() << "change " << change;
    return QGraphicsItem::itemChange(change, value);
}
