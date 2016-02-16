
#include "frameeditor.h"


FrameEditor::FrameEditor()
{
    setupUi();
}

FrameEditor::~FrameEditor()
{

}

void FrameEditor::setupUi()
{
    if (this->objectName().isEmpty())
    this->resize(762, 544);
    centralWidget = new QWidget(this);
    verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    graphicsView = new QGraphicsView(centralWidget);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // sizePolicy.setHorizontalStretch(0);
    // sizePolicy.setVerticalStretch(0);
    // sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
    graphicsView->setSizePolicy(sizePolicy);

    verticalLayout->addWidget(graphicsView);

    this->setCentralWidget(centralWidget);

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
