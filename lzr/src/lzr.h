
#pragma once

#include <QtWidgets>
#include "frameeditor.h"
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

    FrameEditor* editor;

    ToolDock* tools;
    ColorDock* color;
    PathDock* paths;

    /*
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    */
};
