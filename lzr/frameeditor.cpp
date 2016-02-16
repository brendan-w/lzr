
#include "frameeditor.h"


FrameEditor::FrameEditor()
{
    setupUi();

    //add some points to test with
    frame->addPoint();
    frame->addPoint();
}

FrameEditor::~FrameEditor()
{

}

void FrameEditor::setupUi()
{
    if (this->objectName().isEmpty())
    this->resize(762, 544);

    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    
    frame = new FrameView(centralWidget);
    verticalLayout->addWidget(frame);

    menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, 762, 20));
    this->setMenuBar(menuBar);

    mainToolBar = new QToolBar(this);
    this->addToolBar(Qt::TopToolBarArea, mainToolBar);

    statusBar = new QStatusBar(this);
    this->setStatusBar(statusBar);

    color = new ColorDock(this);
    this->addDockWidget(Qt::LeftDockWidgetArea, color);
}
