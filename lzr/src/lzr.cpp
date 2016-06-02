
#include "lzr.h"


LZR::LZR()
{
    setupUi();

    //add some points to test with
    frame->addPoint();
    frame->addPoint();
}

LZR::~LZR()
{

}

void LZR::setupUi()
{
    if(objectName().isEmpty())
        resize(1024, 768);

    setCentralWidget(stack = new QStackedWidget(this));
    stack->addWidget(frame = new FrameView(stack));

    addDockWidget(Qt::LeftDockWidgetArea, tools = new ToolDock(this));
    addDockWidget(Qt::LeftDockWidgetArea, color = new ColorDock(this));

    /*
    setMenuBar(menuBar = new QMenuBar(this));
    menuBar->setGeometry(QRect(0, 0, 762, 20));

    addToolBar(Qt::TopToolBarArea, mainToolBar = new QToolBar(this));
    setStatusBar(statusBar = new QStatusBar(this));
    */
}
