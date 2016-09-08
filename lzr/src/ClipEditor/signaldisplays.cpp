

#include "signaldisplays.h"

/*
 * CONSTANT
 */

ConstantDisplay::ConstantDisplay(Signal* s, QWidget* parent) : QWidget(parent),
                                                                           signal((ConstantSignal*) s),
                                                                           step(0.01)
{
    setLayout(hbox = new QHBoxLayout(this));
    hbox->addWidget(digits = new QLabel(this));
    hbox->addWidget(slider = new QSlider(Qt::Horizontal, this), 1);

    hbox->setContentsMargins(10, 0, 0, 0);

    digits->setFixedWidth(40);
    slider->setMinimum((int) signal->min() / step);
    slider->setMaximum((int) signal->max() / step);

    Time t;
    double value = signal->value(t);
    slider->setSliderPosition(value / step);
    digits->setText(QString::number(value));

    connect(slider, SIGNAL(valueChanged(int)),
            this, SLOT(setValue(int)));
}

void ConstantDisplay::setValue(int v)
{
    double value = v * step;
    signal->setValue(value);
    digits->setText(QString::number(value));
}


/*
 * CURVE
 */

