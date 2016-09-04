
#pragma once

#include <QtWidgets>
#include "frame.h"
#include "ClipEditor/clipeditor.h"
#include "FrameEditor/frameeditorview.h"
#include "FrameEditor/frameeditorscene.h"
#include "FrameEditor/colordock.h"
#include "FrameEditor/tooldock.h"
#include "FrameEditor/pathdock.h"
#include "FrameEditor/settingsdock.h"


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
    ClipEditor* clip_editor;
};
