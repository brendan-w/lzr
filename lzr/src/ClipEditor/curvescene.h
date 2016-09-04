
#pragma once

#include <QtWidgets>
#include <QDebug>


class CurveScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CurveScene(QWidget* parent = 0);
    ~CurveScene();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

private:

};
