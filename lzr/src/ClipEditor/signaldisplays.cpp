

#include "signaldisplays.h"
#include "../settings.h"

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

#define ZOOM_FACTOR 1.2

CurveDisplay::CurveDisplay(Signal* s, QWidget* parent) : QGraphicsView(parent),
                                                         signal((CurveSignal*) s)
{
    // setRenderHint(QPainter::Antialiasing);
    setFrameStyle(QFrame::NoFrame);
    //disable all scroll bars
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //when points move, the connecting lines must be also redrawn
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    setFixedHeight(200);
}

void CurveDisplay::keyPressEvent(QKeyEvent* e)
{
    if(!e->isAutoRepeat())
    {
        if(e->key() == EDITOR_PAN_KEY)
        {
            setInteractive(false);
            setDragMode(ScrollHandDrag);
        }
    }

    QGraphicsView::keyPressEvent(e);
}

void CurveDisplay::keyReleaseEvent(QKeyEvent* e)
{
    if(!e->isAutoRepeat())
    {
        if(e->key() == EDITOR_PAN_KEY)
        {
            setInteractive(true);
            setDragMode(NoDrag);
        }
    }

    QGraphicsView::keyReleaseEvent(e);
}

void CurveDisplay::wheelEvent(QWheelEvent* event)
{
    double factor = ZOOM_FACTOR;

    if(event->angleDelta().y() < 0)
        factor = 1.0 / ZOOM_FACTOR;

    scale(factor, 1);
}
