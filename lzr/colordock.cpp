
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
    layout->addWidget(view);

    scene = new QGraphicsScene(content);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setScene(scene);

    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
}
