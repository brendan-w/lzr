
#include "colordock.h"



ColorDock::ColorDock(QWidget* parent) : QDockWidget(parent)
{
    setWindowTitle("Color");
    setFeatures(QDockWidget::DockWidgetMovable);

    content = new QWidget();
    setWidget(content);

    layout = new QVBoxLayout(content);

    layout->addWidget(view = new QGraphicsView(content));
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameShape(QFrame::NoFrame);
    view->setFocusPolicy(Qt::NoFocus);

    scene = new QGraphicsScene(content);
    view->setScene(scene);

    layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    colors = new ColorSwatch(QPixmap("colors.png"));
    scene->addItem(colors);
    view->setFixedSize(colors->pixmap().size());

    indicator = new QGraphicsRectItem();
    scene->addItem(indicator);

    //listen to color changes
    connect(colors, SIGNAL(color_selected(QColor, QRect)),
            this, SLOT(color_selected(QColor, QRect)));

    colors->select_color_at(QPoint(0, 0));
}

void ColorDock::color_selected(QColor c, QRect rect)
{
    color = c;

    //indicate with white, if it's a dark color
    QPen pen(Qt::black, 0);
    if(color.lightness() <= 127)
        pen.setColor(Qt::white);

    indicator->setPen(pen);
    indicator->setRect(rect);

    emit color_changed(color);
}

void ColorDock::emit_color_changed()
{
    emit color_changed(color);
}





ColorSwatch::ColorSwatch(const QPixmap& pixmap, QGraphicsItem* parent) :
    QGraphicsPixmapItem(pixmap, parent)
{
    //save a copy of the swatch so we don't have to do this on every click
    swatch = pixmap.toImage();
}

void ColorSwatch::select_color_at(const QPoint pos)
{
    //get the color under the mouse
    QColor color = swatch.pixelColor(pos);
    QRect rect = rect_around_color(pos, color);
    emit color_selected(color, rect);
}


void ColorSwatch::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    select_color_at(event->pos().toPoint());
}

QRect ColorSwatch::rect_around_color(const QPoint pos, const QColor color)
{
    QRect rect;

    //determine the bounding rectangle for this color swatch
    QSize size = pixmap().size();
    QColor test;
    int i;

    //left bound
    test = color;
    i = pos.x();
    while(test == color)
    {
        //record the current valid pixel extent
        rect.setX(i);
        //increment/finish
        if((--i) < 0) break;
        //lookup the next pixel
        test = swatch.pixelColor(i, pos.y());
    }

    //upper bound
    test = color;
    i = pos.y();
    while(test == color)
    {
        rect.setY(i);
        if((--i) < 0) break;
        test = swatch.pixelColor(pos.x(), i);
    }

    //right bound
    test = color;
    i = pos.x();
    while(test == color)
    {
        rect.setWidth(i - rect.x());
        if((++i) >= size.width()) break;
        test = swatch.pixelColor(i, pos.y());
    }

    //lower bound
    test = color;
    i = pos.y();
    while(test == color)
    {
        rect.setHeight(i - rect.y());
        if((++i) >= size.height()) break;
        test = swatch.pixelColor(pos.x(), i);
    }

    return rect;
}
