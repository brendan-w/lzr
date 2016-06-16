
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
    paths->setContextMenuPolicy(Qt::CustomContextMenu);
    //paths->setResizeMode(QListView::Adjust);

    paths->setFixedWidth(PATH_DELEGATE_SIZE);
    content->setFixedWidth(PATH_DELEGATE_SIZE + 20);

    connect(paths, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showContextMenu(const QPoint&)));
}

void PathDock::setModel(QAbstractItemModel* model)
{
    paths->setModel(model);
}

QItemSelectionModel* PathDock::selectionModel()
{
    return paths->selectionModel();
}

/*
 * Slots
 */

void PathDock::showContextMenu(const QPoint& mouse)
{
    QMenu contextMenu;

    if(paths->selectionModel()->selectedRows().size() == 0)
    {
        contextMenu.addAction("Add Path");
    }
    else
    {
        contextMenu.addAction("Duplicate");
        contextMenu.addAction("Mirror Horizontally");
        contextMenu.addAction("Mirror Vertically");
        contextMenu.addAction("Delete");
    }

    contextMenu.exec(paths->mapToGlobal(mouse));
}
