

#include "signaldisplays.h"


ConstantSignalDisplay::ConstantSignalDisplay(Signal* s, QWidget* parent) : QWidget(parent),
                                                                           signal((ConstantSignal*) s)
{
    spinner = new QDoubleSpinBox(this);
    spinner->setRange(0.01, 2.0);
    spinner->setSingleStep(0.01);
    spinner->setFocusPolicy(Qt::NoFocus);

    // connect(spinner, SIGNAL(valueChanged(double)),
            // this, SIGNAL(constant_changed(double)));
}

QSize ConstantSignalDisplay::sizeHint()
{
    return spinner->sizeHint();
}

void ConstantSignalDisplay::constant_changed(double value)
{
    signal->value = value;
}
