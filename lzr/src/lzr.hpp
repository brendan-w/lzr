
#pragma once

#include <QtWidgets>
#include "frame.hpp"
#include "clip.hpp"
#include "ClipEditor/clipeditor.hpp"
#include "FrameEditor/frameeditorview.hpp"
#include "FrameEditor/frameeditorscene.hpp"
#include "FrameEditor/colordock.hpp"
#include "FrameEditor/tooldock.hpp"
#include "FrameEditor/pathdock.hpp"
#include "FrameEditor/settingsdock.hpp"


class LZR : public QMainWindow
{
    Q_OBJECT

public:
    explicit LZR();
    ~LZR();

private:
    void setupUi();
    void show_frameeditor(Frame* frame);
    void show_clipeditor(Clip* clip);

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
