
#pragma once

#include <QtWidgets>
#include <QDebug>


class TimelineView : public QGraphicsView
{
    Q_OBJECT

public:
    TimelineView(QWidget* parent = 0);
    void reset();

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void wheelEvent(QWheelEvent* e);
};
