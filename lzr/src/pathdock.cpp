
#include "pathdock.h"
#include "pathdelegate.h"


PathDock::PathDock(QWidget* parent) : QDockWidget(parent)
{
    setWindowTitle("Paths");
    setFeatures(QDockWidget::DockWidgetMovable);

    setWidget(content = new QWidget());

    layout = new QVBoxLayout(content);
    layout->addWidget(paths = new QListView());

    paths->setEditTriggers(QAbstractItemView::NoEditTriggers);
    paths->setSelectionMode(QAbstractItemView::ExtendedSelection);
    paths->setFrameShape(QFrame::NoFrame);
    paths->setUniformItemSizes(true);
    paths->setItemDelegate(new PathDelegate());
    paths->setFocusPolicy(Qt::NoFocus);
    //paths->setResizeMode(QListView::Adjust);

    paths->setFixedWidth(PATH_DELEGATE_SIZE);
    content->setFixedWidth(PATH_DELEGATE_SIZE + 20);
}

void PathDock::setModel(QAbstractItemModel* model)
{
    paths->setModel(model);
}

QItemSelectionModel* PathDock::selectionModel()
{
    return paths->selectionModel();
}
