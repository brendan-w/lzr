
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
