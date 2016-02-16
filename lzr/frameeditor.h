#ifndef FRAMEEDITOR_H
#define FRAMEEDITOR_H

#include <QtWidgets>
#include "frameview.h"
#include "colordock.h"


class FrameEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit FrameEditor();
    ~FrameEditor();

private:
    void setupUi();

    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    FrameView* frame;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    ColorDock* color;
};

#endif // FRAMEEDITOR_H
