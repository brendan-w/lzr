
#pragma once
#include <QtWidgets>
#include "colorswatch.h"


class ColorDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit ColorDock(QWidget* parent = 0);
    virtual ~ColorDock();

public slots:
    void setColor(QRgb color);

private:
    void setupUi();

    QWidget* content;
    QVBoxLayout* layout;
    QGraphicsView* view;
    QGraphicsScene* scene;
    ColorSwatch* colors;
};
