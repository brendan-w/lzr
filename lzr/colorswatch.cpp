
#include "colorswatch.h"


ColorSwatch::ColorSwatch(const QPixmap& pixmap, QGraphicsItem* parent) :
    QGraphicsPixmapItem(pixmap, parent)
{
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
