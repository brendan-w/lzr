

#include "signaldisplays.h"


ConstantSignalDisplay::ConstantSignalDisplay(Signal* s, QWidget* parent) : QWidget(parent),
                                                                           signal((ConstantSignal*) s),
                                                                           step(0.01),
                                                                           tickStep(1)
{
    setLayout(hbox = new QHBoxLayout(this));
    hbox->addWidget(digits = new DoubleDisplay(s, this));
    hbox->addWidget(slider = new QSlider(Qt::Horizontal, this), 1);

    hbox->setContentsMargins(10, 0, 0, 0);

    slider->setMinimum((int) signal->min() / step);
    slider->setMaximum((int) signal->max() / step);
    slider->setTickInterval((int) tickStep / step);
    // slider->setTickPosition(QSlider::TicksBelow);

    Time t;
    double value = signal->value(t) / step;
    slider->setSliderPosition(value / step);
    digits->setText(QString::number(value));

    connect(slider, SIGNAL(valueChanged(int)),
            this, SLOT(setValue(int)));
}

void ConstantSignalDisplay::setValue(int v)
{
    double value = v * step;
    signal->setValue(value);
    digits->setText(QString::number(value));
}
