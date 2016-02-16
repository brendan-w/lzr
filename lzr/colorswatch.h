
#pragma once
#include <QtWidgets>


class ColorSwatch : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit ColorSwatch(const QPixmap& pixmap, QGraphicsItem* parent = 0);
    virtual ~ColorSwatch();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

signals:
    void newColorSelected(QRgb color);

private:
    QImage swatch;
};
