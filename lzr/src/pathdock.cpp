
#include "pathdock.h"


PathDock::PathDock(QWidget* parent) : QDockWidget(parent)
{
    setWindowTitle("Paths");
    setFeatures(QDockWidget::DockWidgetMovable);

    content = new QWidget();
    setWidget(content);
}

PathDock::~PathDock()
{

}
