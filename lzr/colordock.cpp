
#include <QDebug>
#include "colordock.h"



ColorDock::ColorDock(QWidget* parent) : QDockWidget(parent)
{
    setupUi();
    colors = new ColorSwatch(QPixmap("colors.png"));
    scene->addItem(colors);
    view->setFixedSize(colors->pixmap().size());

    //listen to color changes
    connect(colors, SIGNAL(newColorSelected(QRgb)),
            this, SLOT(setColor(QRgb)));
}

ColorDock::~ColorDock()
{

}

void ColorDock::setColor(QRgb color)
{
    qDebug() << "new color" << color;
}

void ColorDock::setupUi()
{
    this->setWindowTitle("Color");
    content = new QWidget();
    this->setWidget(content);
    
    layout = new QVBoxLayout(content);

    view = new QGraphicsView(content);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameShape(QFrame::NoFrame);
    layout->addWidget(view);

    scene = new QGraphicsScene(content);
    view->setScene(scene);

    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
}








ColorSwatch::ColorSwatch(const QPixmap& pixmap, QGraphicsItem* parent) :
    QGraphicsPixmapItem(pixmap, parent)
{
    //save a copy of the swatch so we don't have to do this on every click
    swatch = pixmap.toImage();
}

ColorSwatch::~ColorSwatch()
{

}

void ColorSwatch::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QPointF mousePosition = event->pos();
    QRgb color = swatch.pixel(mousePosition.x(), mousePosition.y());
    emit newColorSelected(color);
}
