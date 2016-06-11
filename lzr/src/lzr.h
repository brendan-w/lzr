
#pragma once

#include <QtWidgets>
#include "frameeditorview.h"
#include "frameeditorscene.h"
#include "frame.h"
#include "colordock.h"
#include "tooldock.h"
#include "pathdock.h"


class LZR : public QMainWindow
{
    Q_OBJECT

public:
    explicit LZR();
    ~LZR();

private:
    void setupUi();
    void show_frameeditor(Frame* frame);

    QStackedWidget* stack;

    FrameEditor* editor_view;
    FrameScene* editor_scene;

    ToolDock* tools;
    ColorDock* color;
    PathDock* paths;

    /*
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    */
};
