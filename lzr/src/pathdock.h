
#pragma once
#include <QtWidgets>
#include <QDebug>
#include "frame.h"


class PathDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit PathDock(QWidget* parent = 0);
    void setModel(Frame* frame);
    QItemSelectionModel* selectionModel();

signals:
    void select_all_points();
    void deselect_all_points();

public slots:
    void showContextMenu(const QPoint& mouse);
    void duplicate();
    void mirror_h();
    void mirror_v();
    void remove();

private:
    Frame* frame;

    QWidget* content;
    QVBoxLayout* layout;
    QListView* paths;

    //context menus
    QMenu menu_for_selection;
    QMenu menu_for_no_selection;
};
