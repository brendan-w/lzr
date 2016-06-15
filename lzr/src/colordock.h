
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
    void select_color_at(const QPoint pos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

signals:
    void color_selected(QColor c, QRect rect);

private:
    QImage swatch;
    QRect rect_around_color(const QPoint pos, const QColor color);
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
    void color_selected(QColor c, QRect rect);

private:
    QColor color; //the currently selected color

    QWidget* content;
    QVBoxLayout* layout;
    QGraphicsView* view;
    QGraphicsScene* scene;
    ColorSwatch* colors;
    QGraphicsRectItem* indicator;

};
