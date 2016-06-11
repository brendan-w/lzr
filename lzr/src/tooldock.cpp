
#include "tooldock.h"


ToolDock::ToolDock(QWidget* parent) : QDockWidget(parent)
{
    setWindowTitle("Tools");
    setFeatures(QDockWidget::DockWidgetMovable);

    setWidget(content = new QWidget());
    layout = new FlowLayout(content, -1, 0, 0);

    actions = new QActionGroup(content);
    actions->setExclusive(true);

    line      = makeTool(MOVE, "Line draw",    QKeySequence(Qt::Key_L));
    draw      = makeTool(LINE, "Free draw",    QKeySequence(Qt::Key_F));
    move      = makeTool(DRAW, "Move points",  QKeySequence(Qt::Key_M));
    add_point = makeTool(ADD,  "Add point",    QKeySequence(Qt::Key_A));
    del_point = makeTool(DEL,  "Delete point", QKeySequence(Qt::Key_D));

    //set default tool
    line->defaultAction()->toggle();
}

ToolDock::~ToolDock()
{

}

void ToolDock::tool_toggled(bool checked)
{
    if(checked) //ignore the uncheck events from the previous tool
    {
        QAction* action = (QAction*) sender();
        emit tool_changed(action->data().value<tool_t>());
    }
}

QToolButton* ToolDock::makeTool(tool_t tool, const QString& tooltip, const QKeySequence& shortcut)
{
    QToolButton* button = new QToolButton(content);
    QAction* action = new QAction(tooltip, button);

    //load details into the action
    QVariant v;
    v.setValue(tool);
    action->setCheckable(true);
    action->setData(v);
    action->setShortcut(shortcut);

    connect(action, SIGNAL(toggled(bool)),
            this, SLOT(tool_toggled(bool)));

    actions->addAction(action);
    button->setDefaultAction(action);
    layout->addWidget(button);
    return button;
}
