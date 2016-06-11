
#include <QtGui>
#include "frameeditorscene.h"
#include "utils.h"


FrameScene::FrameScene(QWidget *parent) : QGraphicsScene(parent)
{
    //enforce custom coordinate system [-1.0, 1.0]
    //Y is negative to make positive values go upwards
    setSceneRect(-3.0, -3.0, 6.0, 6.0); //bigger than (-1.0, -1.0, 2.0, 2.0) for scrollability
    setBackgroundBrush(Qt::black);
    addItem(grid = new Grid);
}

void FrameScene::setModel(Frame* m)
{
    foreach(Path* path, paths)
    {
        removeItem(path);
        delete path;
    }

    paths.clear();

    //TODO: disconnect signals from the model
    model = m;
    //TODO connect signals

    //create all of the path objects in the model
    for(int i = 0; i < model->rowCount(); i++)
    {
        QModelIndex index = model->index(i);
        lzr::Frame lzr_path = index.data().value<lzr::Frame>();

        Path* path = new Path(index, lzr_path);
        paths.append(path);
        addItem(path);

        connect(path, SIGNAL(changed(Path*)),
                this, SLOT(path_changed(Path*)));
    }
}

void FrameScene::setSelectionModel(QItemSelectionModel* model)
{
    selection = model;
}

void FrameScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    mouse = e->scenePos();

    if(tool == LINE && selection->hasSelection())
    {
        update();
    }

    QGraphicsScene::mouseMoveEvent(e);
}

void FrameScene::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    if(tool == LINE && selection->hasSelection())
    {
        QModelIndex index = selection->currentIndex();
        Path* path = paths[index.row()];
        Point* old_point = (Point*) path->childItems().back();
        QPointF pos = constrain_to_frame(e->scenePos());
        Point* point = new Point(pos, old_point->getColor());
        path->add_point(point);
    }

    QGraphicsScene::mousePressEvent(e);
}

void FrameScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect); //because we always render the entire scene

    if(tool == LINE && selection->hasSelection())
    {
        //lookup the currently selected path
        QModelIndex index = selection->currentIndex();
        Path* path = paths[index.row()];
        Point* point = (Point*) path->childItems().back();
        QPointF pos = constrain_to_frame(mouse);

        painter->setPen(QPen(Qt::darkGray, 0));
        painter->drawLine(QLineF(point->x(), point->y(), pos.x(), pos.y()));
    }
}



/*
 * Slots
 */

void FrameScene::path_changed(Path* path)
{
    //update the model with the new path data
    QVariant v;
    v.setValue(path->to_LZR());
    model->setData(path->get_index(), v);
}

void FrameScene::tool_changed(tool_t t)
{
    tool = t;
    update();
}

void FrameScene::color_changed(QColor c)
{
    color = c;
}
