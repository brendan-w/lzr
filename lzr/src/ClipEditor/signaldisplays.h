
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "../signals.h"



class ConstantDisplay : public QWidget
{
    Q_OBJECT
public:
    ConstantDisplay(Signal* s, QWidget* parent = 0);

public slots:
    void setValue(int);

private:
    ConstantSignal* signal;
    const double step;

    QHBoxLayout* hbox;
    QLabel* digits;
    QSlider* slider;
};



class CurveDisplay : public QGraphicsView
{
    Q_OBJECT
public:
    CurveDisplay(Signal* s, QWidget* parent = 0);

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void wheelEvent(QWheelEvent* e);
};



class CurveScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CurveScene(Signal* s, QWidget* parent = 0);

protected:
    void drawForeground(QPainter* painter, const QRectF& rect);
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

private:
    CurveSignal* signal;
};
