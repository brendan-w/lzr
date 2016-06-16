

#include "liblzr.h"
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

void PathDock::setModel(Frame* f)
{
    paths->setModel(f);
    frame = f;
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
        contextMenu.addAction("Select All Points");
        contextMenu.addAction("Duplicate", this, SLOT(duplicate()));
        contextMenu.addAction("Mirror Horizontally", this, SLOT(mirror_h()));
        contextMenu.addAction("Mirror Vertically", this, SLOT(mirror_v()));
        contextMenu.addAction("Delete", this, SLOT(remove()));
    }

    contextMenu.exec(paths->mapToGlobal(mouse));
}

void PathDock::duplicate()
{
    foreach(const QModelIndex& index, selectionModel()->selectedRows())
    {
        frame->duplicate(index);
    }
}

void PathDock::mirror_h()
{
    foreach(const QModelIndex& index, selectionModel()->selectedRows())
    {
        lzr::Frame path = frame->get_path(index);
        lzr::mirror(path, lzr::Point(), true, false);
        frame->set_path(index, path);
    }
}

void PathDock::mirror_v()
{
    foreach(const QModelIndex& index, selectionModel()->selectedRows())
    {
        lzr::Frame path = frame->get_path(index);
        lzr::mirror(path, lzr::Point(), false, true);
        frame->set_path(index, path);
    }
}

void PathDock::remove()
{
    foreach(const QModelIndex& index, selectionModel()->selectedRows())
    {
        frame->removeRow(index.row());
    }
}
