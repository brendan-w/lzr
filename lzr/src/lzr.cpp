
#include "lzr.h"


LZR::LZR()
{
    setupUi();

    //add some points to test with
    editor->addPoint();
    editor->addPoint();

    //test the Frame model
    lzr::Frame f;
    f.add(                lzr::Point(0, 0,   255, 255, 255, 255));
    f.add(                lzr::Point(0, 0.5, 255, 255, 255, 255));
    f.add(                lzr::Point(0, 1,   255, 255, 255, 255));
    f.add_with_blank_jump(lzr::Point(1, 0,   255, 255, 255, 255));
    f.add(                lzr::Point(1, 1,   255, 255, 255, 255));

    frame = new Frame(f);

    qDebug() << frame->rowCount();
    QVariant v = frame->data(frame->index(0));
    Path* p = v.value<Path*>();
    qDebug() << p->rowCount();
}

LZR::~LZR()
{

}

void LZR::setupUi()
{
    if(objectName().isEmpty())
        resize(1024, 768);

    setCentralWidget(stack = new QStackedWidget(this));
    stack->addWidget(editor = new FrameEditor(stack));

    addDockWidget(Qt::LeftDockWidgetArea, tools = new ToolDock(this));
    addDockWidget(Qt::LeftDockWidgetArea, color = new ColorDock(this));
    addDockWidget(Qt::RightDockWidgetArea, paths = new PathDock(this));

    /*
    setMenuBar(menuBar = new QMenuBar(this));
    menuBar->setGeometry(QRect(0, 0, 762, 20));

    addToolBar(Qt::TopToolBarArea, mainToolBar = new QToolBar(this));
    setStatusBar(statusBar = new QStatusBar(this));
    */
}
