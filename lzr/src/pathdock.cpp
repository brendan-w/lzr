

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

    //layout constrictions
    paths->setFixedWidth(PATH_DELEGATE_SIZE);
    content->setFixedWidth(PATH_DELEGATE_SIZE + 20);

    //make the menus
    menu_for_selection.addAction("Select All Points", this, SIGNAL(select_all_points()));
    menu_for_selection.addAction("Deselect All Points", this, SIGNAL(deselect_all_points()));
    menu_for_selection.addSeparator();
    menu_for_selection.addAction("Duplicate", this, SLOT(duplicate()));
    menu_for_selection.addAction("Mirror Horizontally", this, SLOT(mirror_h()));
    menu_for_selection.addAction("Mirror Vertically", this, SLOT(mirror_v()));
    menu_for_selection.addSeparator();
    menu_for_selection.addAction("Delete", this, SLOT(remove()));

    menu_for_no_selection.addAction("Add Path", this, SLOT(add()));

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
    if(paths->indexAt(mouse).isValid())
        menu_for_selection.exec(paths->mapToGlobal(mouse));
    else
        menu_for_no_selection.exec(paths->mapToGlobal(mouse));
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
    //use persistent indexes to keep things consistent during deletion
    QList<QPersistentModelIndex> indices;

    foreach(const QModelIndex& index, selectionModel()->selectedRows())
    {
        indices.append(index);
    }

    foreach(const QPersistentModelIndex& index, indices)
    {
        frame->removeRow(index.row());
    }
}

void PathDock::add()
{
    selectionModel()->select(frame->add_path(),
                             QItemSelectionModel::ClearAndSelect);
}
