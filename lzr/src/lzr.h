
#pragma once

#include <QtWidgets>
#include "frameview.h"
#include "colordock.h"
#include "tooldock.h"


class LZR : public QMainWindow
{
    Q_OBJECT

public:
    explicit LZR();
    ~LZR();

private:
    void setupUi();

    QStackedWidget* stack;

    FrameView* frame;

    ToolDock* tools;
    ColorDock* color;

    /*
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    */
};
