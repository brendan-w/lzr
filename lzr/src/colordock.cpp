
#include "colordock.h"



ColorDock::ColorDock(QWidget* parent) : QDockWidget(parent)
{
    setWindowTitle("Color");
    setFeatures(QDockWidget::DockWidgetMovable);

    content = new QWidget();
    setWidget(content);

    layout = new QVBoxLayout(content);

    view = new QGraphicsView(content);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameShape(QFrame::NoFrame);
    layout->addWidget(view);

    scene = new QGraphicsScene(content);
    view->setScene(scene);

    layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    colors = new ColorSwatch(QPixmap("colors.png"));
    scene->addItem(colors);
    view->setFixedSize(colors->pixmap().size());

    indicator = new QGraphicsRectItem();
    scene->addItem(indicator);

    //listen to color changes
    connect(colors, SIGNAL(color_selected(QRgb, QRect)),
            this, SLOT(color_selected(QRgb, QRect)));
}

void ColorDock::color_selected(QRgb rgb, QRect rect)
{
    color.setRgb(rgb);

    //indicate with white, if it's a dark color
    QPen pen(Qt::black, 0);
    if(color.lightness() <= 127)
        pen.setColor(Qt::white);

    indicator->setPen(pen);
    indicator->setRect(rect);

    emit color_changed(color);
}








ColorSwatch::ColorSwatch(const QPixmap& pixmap, QGraphicsItem* parent) :
    QGraphicsPixmapItem(pixmap, parent)
{
    //save a copy of the swatch so we don't have to do this on every click
    swatch = pixmap.toImage();
}

void ColorSwatch::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    const int x = event->pos().x();
    const int y = event->pos().y();

    //get the color under the mouse
    QRgb color = swatch.pixel(x, y);
    QRect rect;

    //determine the bounding rectangle for this color swatch
    QSize size = pixmap().size();
    QRgb test;
    int i;

    //left bound
    test = color;
    i = x;
    while(test == color)
    {
        //record the current valid pixel extent
        rect.setX(i);
        //increment/finish
        if((--i) < 0) break;
        //lookup the next pixel
        test = swatch.pixel(i, y);
    }

    //upper bound
    test = color;
    i = y;
    while(test == color)
    {
        rect.setY(i);
        if((--i) < 0) break;
        test = swatch.pixel(x, i);
    }

    //right bound
    test = color;
    i = x;
    while(test == color)
    {
        rect.setWidth(i - rect.x());
        if((++i) >= size.width()) break;
        test = swatch.pixel(i, y);
    }

    //lower bound
    test = color;
    i = y;
    while(test == color)
    {
        rect.setHeight(i - rect.y());
        if((++i) >= size.height()) break;
        test = swatch.pixel(x, i);
    }

    emit color_selected(color, rect);
}
