
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "../signals.h"


class DoubleDisplay : public QLabel
{
    Q_OBJECT
public:
    DoubleDisplay(Signal* s, QWidget* parent = 0) : QLabel(parent),
                                                    signal((DoubleSignal*) s)
    {
        setFixedWidth(40);
        connect(signal, SIGNAL(valueChanged(double)),
                this, SLOT(valueChanged(double)));

    };

public slots:
    void valueChanged(double v)
    {
        setText(QString::number(v));
    };

private:
    DoubleSignal* signal;
};



class ConstantSignalDisplay : public QWidget
{
    Q_OBJECT
public:
    ConstantSignalDisplay(Signal* s, QWidget* parent = 0);

public slots:
    void setValue(int);

private:
    ConstantSignal* signal;
    const double step;

    QHBoxLayout* hbox;
    DoubleDisplay* digits;
    QSlider* slider;
};
