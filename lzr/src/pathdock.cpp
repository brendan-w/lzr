
#include "pathdock.h"


PathDock::PathDock(QWidget* parent) : QDockWidget(parent)
{
    setWindowTitle("Paths");
    setFeatures(QDockWidget::DockWidgetMovable);

    content = new QWidget();
    setWidget(content);

    layout = new QVBoxLayout(content);
    layout->addWidget(paths = new QListView());
}

PathDock::~PathDock()
{

}
