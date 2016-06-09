
#include "pathdock.h"
#include "pathdelegate.h"


PathDock::PathDock(QWidget* parent) : QDockWidget(parent)
{
    setWindowTitle("Paths");
    setFeatures(QDockWidget::DockWidgetMovable);

    content = new QWidget();
    setWidget(content);

    layout = new QVBoxLayout(content);
    layout->addWidget(paths = new QListView());

    paths->setEditTriggers(QAbstractItemView::NoEditTriggers);
    paths->setFrameShape(QFrame::NoFrame);
    paths->setItemDelegate(new PathDelegate);
}

PathDock::~PathDock()
{

}

void PathDock::setModel(QAbstractItemModel* model)
{
    paths->setModel(model);
}
