
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

private:
    CurveSignal* signal;
};
