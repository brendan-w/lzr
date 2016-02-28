
#pragma once
#include <QtWidgets>
#include <QDebug>
#include "flowlayout.h"



enum tool_t {
    MOVE,
    LINE,
    DRAW,
    ADD,
    DEL
};



class ToolDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit ToolDock(QWidget* parent = 0);
    virtual ~ToolDock();

signals:
    void tool_changed(tool_t tool);

public slots:
    void set_tool(tool_t tool);

private:
    QToolButton* makeTool();

    QWidget* content;
    FlowLayout* layout;

    QToolButton* move;
    QToolButton* line;
    QToolButton* draw;
    QToolButton* add_point;
    QToolButton* del_point;
};
