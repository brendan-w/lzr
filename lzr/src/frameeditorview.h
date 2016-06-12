
#pragma once

#include <QtWidgets>
#include <QDebug>

/*
 * Editor for Frame models
 * this should really be a subclass of QAbstractItemView,
 * but that was messier than neccessary.
 */
class FrameEditor : public QGraphicsView
{
    Q_OBJECT

public:
    FrameEditor(QWidget* parent = 0);
    void reset();

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void wheelEvent(QWheelEvent* e);
};
