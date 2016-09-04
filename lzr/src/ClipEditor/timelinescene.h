
#pragma once

#include <QtWidgets>
#include <QDebug>


class TimelineScene : public QGraphicsScene
{
    Q_OBJECT

public:
    TimelineScene(QWidget* parent = 0);
    ~TimelineScene();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

private:

};
