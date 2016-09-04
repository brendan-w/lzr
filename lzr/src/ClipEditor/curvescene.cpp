
#include <QtGui>
#include "curvescene.h"
#include "../utils.h"
#include "../settings.h"

CurveScene::CurveScene(QWidget *parent) : QGraphicsScene(parent)
{
    //enforce custom coordinate system [-1.0, 1.0]
    //Y is negative to make positive values go upwards
    setSceneRect(-3.0, -3.0, 6.0, 6.0); //bigger than (-1.0, -1.0, 2.0, 2.0) for scrollability
    setBackgroundBrush(Qt::black);
}

CurveScene::~CurveScene()
{

}

void CurveScene::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    QGraphicsScene::mousePressEvent(e);
}

void CurveScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
    QGraphicsScene::mouseReleaseEvent(e);
}

void CurveScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    QGraphicsScene::mouseMoveEvent(e);
}

void CurveScene::keyPressEvent(QKeyEvent* e)
{
    QGraphicsScene::keyPressEvent(e);
}

void CurveScene::keyReleaseEvent(QKeyEvent* e)
{
    QGraphicsScene::keyReleaseEvent(e);
}
