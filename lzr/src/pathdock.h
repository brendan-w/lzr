
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
};
