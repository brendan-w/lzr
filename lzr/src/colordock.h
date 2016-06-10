
#pragma once
#include <QtWidgets>
#include <QDebug>


/*
    The swatch (image) of colors
*/
class ColorSwatch : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit ColorSwatch(const QPixmap& pixmap, QGraphicsItem* parent = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

signals:
    void color_selected(QRgb rgb, QRect rect);

private:
    QImage swatch;
};



/*
    The dock widget itself
*/
class ColorDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit ColorDock(QWidget* parent = 0);

signals:
    void color_changed(QColor color);

public slots:
    void color_selected(QRgb rgb, QRect rect);

private:
    QColor color; //the currently selected color

    QWidget* content;
    QVBoxLayout* layout;
    QGraphicsView* view;
    QGraphicsScene* scene;
    ColorSwatch* colors;
    QGraphicsRectItem* indicator;

};
