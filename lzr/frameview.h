
#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QDebug>



class FrameView : public QGraphicsView
{
    Q_OBJECT

public:
    FrameView(QWidget* parent = 0);
    ~FrameView();

    void addPoint();
    QSize sizeHint();


protected:
    void resizeEvent(QResizeEvent* e);

private:
    QGraphicsScene* scene;
};
