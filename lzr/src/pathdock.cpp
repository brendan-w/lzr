
#include "pathdock.h"


PathDock::PathDock(QWidget* parent) : QDockWidget(parent)
{
    this->setWindowTitle("Paths");
    this->setFeatures(QDockWidget::DockWidgetMovable);

    content = new QWidget();
    this->setWidget(content);
}

PathDock::~PathDock()
{

}
