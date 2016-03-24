
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
    QToolButton* makeTool(const QString& tooltip);

    QWidget* content;
    FlowLayout* layout;

    QToolButton* line;
    QToolButton* draw;
    QToolButton* move;
    QToolButton* add_point;
    QToolButton* del_point;
};
