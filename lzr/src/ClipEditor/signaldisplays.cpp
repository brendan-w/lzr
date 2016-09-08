

#include "signaldisplays.h"


ConstantSignalDisplay::ConstantSignalDisplay(Signal* s, QWidget* parent) : QWidget(parent),
                                                                           signal((ConstantSignal*) s),
                                                                           step(0.01)
{
    setLayout(hbox = new QHBoxLayout(this));
    hbox->addWidget(digits = new DoubleDisplay(s, this));
    hbox->addWidget(slider = new QSlider(Qt::Horizontal, this), 1);

    hbox->setContentsMargins(10, 0, 0, 0);

    slider->setMinimum((int) signal->min() / step);
    slider->setMaximum((int) signal->max() / step);

    Time t;
    double value = signal->value(t);
    slider->setSliderPosition(value / step);
    digits->valueChanged(value);

    connect(slider, SIGNAL(valueChanged(int)),
            this, SLOT(setValue(int)));
}

void ConstantSignalDisplay::setValue(int v)
{
    signal->setValue(v * step);
}
