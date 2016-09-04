
#include <QtGui>
#include "timelinescene.h"
#include "../utils.h"
#include "../settings.h"

TimelineScene::TimelineScene(QWidget *parent) : QGraphicsScene(parent)
{
    //enforce custom coordinate system [-1.0, 1.0]
    //Y is negative to make positive values go upwards
    setSceneRect(-3.0, -3.0, 6.0, 6.0); //bigger than (-1.0, -1.0, 2.0, 2.0) for scrollability
    setBackgroundBrush(Qt::black);
}

TimelineScene::~TimelineScene()
{

}

void TimelineScene::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    QGraphicsScene::mousePressEvent(e);
}

void TimelineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
    QGraphicsScene::mouseReleaseEvent(e);
}

void TimelineScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    QGraphicsScene::mouseMoveEvent(e);
}

void TimelineScene::keyPressEvent(QKeyEvent* e)
{
    QGraphicsScene::keyPressEvent(e);
}

void TimelineScene::keyReleaseEvent(QKeyEvent* e)
{
    QGraphicsScene::keyReleaseEvent(e);
}
