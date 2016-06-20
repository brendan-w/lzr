
#pragma once

#include <QtWidgets>
#include "frameeditorview.h"
#include "frameeditorscene.h"
#include "frame.h"
#include "colordock.h"
#include "tooldock.h"
#include "pathdock.h"
#include "settingsdock.h"


class LZR : public QMainWindow
{
    Q_OBJECT

public:
    explicit LZR();
    ~LZR();

private:
    void setupUi();
    void show_frameeditor(Frame* frame);
    void show_clipeditor();

    QStackedWidget* stack;

    /*
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    */

    //FRAME EDITOR
    FrameEditor* editor_view;
    FrameScene* editor_scene;

    ToolDock* tools;
    ColorDock* color;
    PathDock* paths;
    SettingsDock* settings;

    //CLIP EDITOR
    QSplitter* clip_main_split;
    QSplitter* clip_sub_split;
};
