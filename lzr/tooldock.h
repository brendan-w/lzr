
#pragma once
#include <QtWidgets>
#include <QDebug>
#include "flowlayout.h"

/*
    The dock widget itself
*/
class ToolDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit ToolDock(QWidget* parent = 0);
    virtual ~ToolDock();

private:
    QPushButton* makeTool(QString text);

    QWidget* content;
    FlowLayout* layout;

    QPushButton* move;
    QPushButton* line;
    QPushButton* draw;
    QPushButton* add_point;
    QPushButton* del_point;
};
