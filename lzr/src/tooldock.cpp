
#include "tooldock.h"


ToolDock::ToolDock(QWidget* parent) : QDockWidget(parent)
{
    setWindowTitle("Tools");
    setFeatures(QDockWidget::DockWidgetMovable);

    setWidget(content = new QWidget());
    layout = new FlowLayout(content, -1, 0, 0);

    line = makeTool("Line draw");
    draw = makeTool("Free draw");
    move = makeTool("Move points");
    add_point = makeTool("Add point");
    del_point = makeTool("Delete point");

    //set default tool
    line->setChecked(true);
}

ToolDock::~ToolDock()
{

}

void ToolDock::set_tool(tool_t tool)
{
    switch(tool)
    {
        case LINE: line->setChecked(true); break;
        case DRAW: draw->setChecked(true); break;
        case MOVE: move->setChecked(true); break;
        case ADD:  add_point->setChecked(true); break;
        case DEL:  del_point->setChecked(true); break;
    }
}

QToolButton* ToolDock::makeTool(const QString& tooltip)
{
    QToolButton* tool = new QToolButton(content);
    tool->setCheckable(true);
    tool->setAutoExclusive(true);
    tool->setToolTip(tooltip);
    layout->addWidget(tool);
    return tool;
}
