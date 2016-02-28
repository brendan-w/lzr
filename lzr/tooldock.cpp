
#include "tooldock.h"


ToolDock::ToolDock(QWidget* parent) : QDockWidget(parent)
{
    this->setWindowTitle("Tools");
    this->setFeatures(QDockWidget::DockWidgetMovable |
                      QDockWidget::DockWidgetFloatable);

    content = new QWidget();
    this->setWidget(content);
    
    layout = new FlowLayout(content);

    move = makeTool("Move");
    line = makeTool("Line");
    draw = makeTool("Draw");
    add_point = makeTool("Add Point");
    del_point = makeTool("Del Point");

    layout->addWidget(move);
    layout->addWidget(line);
    layout->addWidget(draw);
    layout->addWidget(add_point);
    layout->addWidget(del_point);
}

ToolDock::~ToolDock()
{

}

QPushButton* ToolDock::makeTool(QString text)
{
    QPushButton* button = new QPushButton(text, content);
    button->setCheckable(true);
    button->setAutoExclusive(true);
    return button;
}
