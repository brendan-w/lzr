

#include "signaldisplays.hpp"
#include "../settings.hpp"

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
    digits->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
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

CurveDisplay::CurveDisplay(Signal* s, QWidget* parent) : QGraphicsView(parent)
{
    // setRenderHint(QPainter::Antialiasing);
    setFixedHeight(200);
    setFrameStyle(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //when points move, the connecting lines must be also redrawn
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    //create the scene
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



//SCENE

CurveScene::CurveScene(Signal* s, QWidget *parent) : QGraphicsScene(parent),
                                                          signal((CurveSignal*) s)
{
    //enforce custom coordinate system [0.0, 1.0]
    //Y is negative to make positive values go upwards
    setSceneRect(0.0, 0.0, 1.0, 1.0);
    setBackgroundBrush(Qt::black);
}

void CurveScene::drawForeground(QPainter* painter, const QRectF& rect)
{
    Q_UNUSED(rect); //because we always render the entire scene

    painter->setPen(Qt::red);

    QRectF r = sceneRect();
    QLineF line;

    int divisions = 4;
    for(int i = 1; i < divisions; i++)
    {
        //normalized
        double n = (double) i / divisions;

        //vertical
        double x = (r.width() * n) + r.left();
        line.setLine(x, r.top(), x, r.bottom());
        painter->drawLine(line); //need to use QLineF to get floating point

        //horizontal
        double y = (r.height() * n) + r.top();
        line.setLine(r.left(), y, r.right(), y);
        painter->drawLine(line);
    }
}

void CurveScene::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    QGraphicsScene::mousePressEvent(e);
}

void CurveScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
    QGraphicsScene::mouseReleaseEvent(e);
}

void CurveScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    QGraphicsScene::mouseMoveEvent(e);
}

void CurveScene::keyPressEvent(QKeyEvent* e)
{
    QGraphicsScene::keyPressEvent(e);
}

void CurveScene::keyReleaseEvent(QKeyEvent* e)
{
    QGraphicsScene::keyReleaseEvent(e);
}
