
#pragma once
#include <QtWidgets>
#include <QDebug>


class PathDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit PathDock(QWidget* parent = 0);
    virtual ~PathDock();
    void setModel(QAbstractItemModel* model);

private:
    QWidget* content;
    QVBoxLayout* layout;
    QListView* paths;
};
