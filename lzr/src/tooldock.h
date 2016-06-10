
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "flowlayout.h"
#include "frameeditor.h"



Q_DECLARE_METATYPE(tool_t);


class ToolDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit ToolDock(QWidget* parent = 0);
    virtual ~ToolDock();

signals:
    void tool_changed(tool_t tool);

public slots:
    void tool_toggled(bool checked);

private:
    QToolButton* makeTool(tool_t tool, const QString& tooltip);

    QWidget* content;
    FlowLayout* layout;

    QActionGroup* actions;
    QToolButton* line;
    QToolButton* draw;
    QToolButton* move;
    QToolButton* add_point;
    QToolButton* del_point;
};
