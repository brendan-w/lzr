
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "../signals.h"


class ConstantSignalDisplay : public QWidget
{
    Q_OBJECT

public:
    ConstantSignalDisplay(Signal* s, QWidget* parent = 0);
    QSize sizeHint();

public slots:
    void constant_changed(double);

private:
    ConstantSignal* signal;
    QDoubleSpinBox* spinner;
};
