
#include "pathdock.h"
#include "pathdelegate.h"


PathDock::PathDock(QWidget* parent) : QDockWidget(parent)
{
    setWindowTitle("Paths");
    setFeatures(QDockWidget::DockWidgetMovable);

    setWidget(content = new QWidget());
    content->setMaximumWidth(PATH_DELEGATE_SIZE + 30);

    layout = new QVBoxLayout(content);
    layout->addWidget(paths = new QListView());

    paths->setEditTriggers(QAbstractItemView::NoEditTriggers);
    paths->setSelectionMode(QAbstractItemView::ExtendedSelection);
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
