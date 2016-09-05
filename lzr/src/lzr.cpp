
#include "lzr.h"

LZR::LZR()
{
    setupUi();
    resize(1024, 600);

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

    //Frame* frame = new Frame(f);
    //show_frameeditor(frame);
    show_clipeditor();
}

LZR::~LZR()
{

}

void LZR::setupUi()
{
    setCentralWidget(stack = new QStackedWidget(this));
    stack->addWidget(editor_view = new FrameEditor(this));
    stack->addWidget(clip_editor = new ClipEditor(this));

    //FRAME EDITOR
    editor_scene = new FrameScene(this);
    editor_view->setScene(editor_scene);

    addDockWidget(Qt::LeftDockWidgetArea, tools = new ToolDock(this));
    addDockWidget(Qt::LeftDockWidgetArea, color = new ColorDock(this));
    addDockWidget(Qt::LeftDockWidgetArea, settings = new SettingsDock(this));
    addDockWidget(Qt::RightDockWidgetArea, paths = new PathDock(this));

    connect(paths, SIGNAL(select_all_points()),
            editor_scene, SLOT(select_all_points()));
    connect(paths, SIGNAL(deselect_all_points()),
            editor_scene, SLOT(deselect_all_points()));
    connect(tools, SIGNAL(tool_changed(tool_t)),
            editor_scene, SLOT(tool_changed(tool_t)));
    connect(color, SIGNAL(color_changed(QColor)),
            editor_scene, SLOT(color_changed(QColor)));
    connect(settings, SIGNAL(grid_changed(int)),
            editor_scene, SLOT(grid_changed(int)));
    connect(settings, SIGNAL(draw_density_changed(double)),
            editor_scene, SLOT(draw_density_changed(double)));
    connect(settings, SIGNAL(brush_size_changed(double)),
            editor_scene, SLOT(brush_size_changed(double)));
    /*
    setMenuBar(menuBar = new QMenuBar(this));
    menuBar->setGeometry(QRect(0, 0, 762, 20));

    addToolBar(Qt::TopToolBarArea, mainToolBar = new QToolBar(this));
    setStatusBar(statusBar = new QStatusBar(this));
    */
}

void LZR::show_frameeditor(Frame* frame)
{
    tools->show();
    color->show();
    settings->show();
    paths->show();

    //set models
    paths->setModel(frame);
    editor_scene->setModel(frame, paths->selectionModel());

    editor_view->reset(); //rearrange the viewport for the new model
    tools->reset(); //reload each dock's settings
    color->reset();

    //show the frame editor
    stack->setCurrentWidget(editor_view);
}

void LZR::show_clipeditor()
{
    tools->hide();
    color->hide();
    settings->hide();
    paths->hide();

    //show the clip editor
    stack->setCurrentWidget(clip_editor);
}
