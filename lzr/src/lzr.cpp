
#include "lzr.h"

LZR::LZR()
{
    setupUi();

    //test the Frame model
    lzr::Frame f;
    f.add(                lzr::Point(-1, -1, 255, 0, 0, 255));
    f.add(                lzr::Point(-1, 0,  255, 0, 0, 255));
    f.add(                lzr::Point(-1, 1,  255, 0, 0, 255));
    f.add_with_blank_jump(lzr::Point(0,  -1, 0, 255, 0, 255));
    f.add(                lzr::Point(0,  1,  0, 255, 0, 255));
    f.add_with_blank_jump(lzr::Point(1,  -1, 0, 0, 255, 255));
    f.add(                lzr::Point(1,  1,  0, 0, 255, 255));
    f.add_with_blank_jump(lzr::Point(-1, 1,  255, 255, 255, 255));
    f.add(                lzr::Point(0,  0,  255, 255, 255, 255));
    f.add(                lzr::Point(-1, -1,  0, 255, 255, 255));

    Frame* frame = new Frame(f);
    show_frameeditor(frame);
}

LZR::~LZR()
{

}

void LZR::setupUi()
{
    setCentralWidget(stack = new QStackedWidget(this));
    stack->addWidget(editor = new FrameEditor(stack));

    addDockWidget(Qt::LeftDockWidgetArea, tools = new ToolDock(this));
    addDockWidget(Qt::LeftDockWidgetArea, color = new ColorDock(this));
    addDockWidget(Qt::RightDockWidgetArea, paths = new PathDock(this));

    connect(tools, SIGNAL(tool_changed(tool_t)),
            editor, SLOT(tool_changed(tool_t)));
    /*
    setMenuBar(menuBar = new QMenuBar(this));
    menuBar->setGeometry(QRect(0, 0, 762, 20));

    addToolBar(Qt::TopToolBarArea, mainToolBar = new QToolBar(this));
    setStatusBar(statusBar = new QStatusBar(this));
    */
}

void LZR::show_frameeditor(Frame* frame)
{
    //TODO:show/hide dock widgets
    paths->setModel(frame);
    editor->setModel(frame);
    stack->setCurrentWidget(editor);
}
