
#include "tooldock.h"


ToolDock::ToolDock(QWidget* parent) : QDockWidget(parent)
{
    this->setWindowTitle("Tools");
    this->setFeatures(QDockWidget::DockWidgetMovable |
                      QDockWidget::DockWidgetFloatable);

    content = new QWidget();
    this->setWidget(content);
    
    layout = new FlowLayout(content, -1, 0, 0);

    move = makeTool();
    line = makeTool();
    draw = makeTool();
    add_point = makeTool();
    del_point = makeTool();

    move->setChecked(true);
}

ToolDock::~ToolDock()
{

}

void ToolDock::set_tool(tool_t tool)
{
    switch(tool)
    {
        case MOVE: move->setChecked(true); break;
        case LINE: line->setChecked(true); break;
        case DRAW: draw->setChecked(true); break;
        case ADD:  add_point->setChecked(true); break;
        case DEL:  del_point->setChecked(true); break;
    }
}

QToolButton* ToolDock::makeTool()
{
    QToolButton* tool = new QToolButton(content);
    tool->setCheckable(true);
    tool->setAutoExclusive(true);
    layout->addWidget(tool);
    return tool;
}
