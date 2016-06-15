
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
    stack->addWidget(editor_view = new FrameEditor(stack));

    editor_scene = new FrameScene(this);
    editor_view->setScene(editor_scene);

    addDockWidget(Qt::LeftDockWidgetArea, tools = new ToolDock(this));
    addDockWidget(Qt::LeftDockWidgetArea, color = new ColorDock(this));
    addDockWidget(Qt::LeftDockWidgetArea, settings = new SettingsDock(this));
    addDockWidget(Qt::RightDockWidgetArea, paths = new PathDock(this));

    connect(tools, SIGNAL(tool_changed(tool_t)),
            editor_scene, SLOT(tool_changed(tool_t)));
    connect(color, SIGNAL(color_changed(QColor)),
            editor_scene, SLOT(color_changed(QColor)));
    connect(settings, SIGNAL(grid_changed(int)),
            editor_scene, SLOT(grid_changed(int)));
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
    tools->set_tool(POINTER);
    paths->setModel(frame);
    editor_scene->setModel(frame, paths->selectionModel());
    editor_view->reset();
    editor_view->scale(200, 200); //TODO, this is a quick hack to make the initial reset() work
    stack->setCurrentWidget(editor_view);
}
