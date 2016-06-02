
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

    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    FrameView* frame;
    QMenuBar *menuBar;
    // QToolBar *mainToolBar;
    QStatusBar *statusBar;

    ToolDock* tools;
    ColorDock* color;
};
