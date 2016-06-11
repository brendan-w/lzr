
#include <QtGui>
#include "frameeditorscene.h"


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

void FrameScene::compensate_for_view_transform(const QTransform& transform)
{
    //when the view is resized, we need update the transform of the individual items
    //to keep them in "pixel" coordinates

    foreach(Path* path, paths)
    {
        //path->setTransform(transform().inverted());
        foreach(QGraphicsItem* point, path->childItems())
        {
            point->setTransform(transform.inverted());
        }
    }
}

void FrameScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    mouse = e->scenePos();
    //update();
    QGraphicsScene::mouseMoveEvent(e);
}

void FrameScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    //painter->setPen(QPen(Qt::gray, 0));
    //painter->drawLine(QLineF(0, 0, mouse.x(), mouse.y()));
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
}

void FrameScene::color_changed(QColor c)
{
    color = c;
}
