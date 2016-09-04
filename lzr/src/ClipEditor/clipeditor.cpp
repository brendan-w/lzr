
#include "clipeditor.h"


ClipEditor::ClipEditor(QWidget* parent) : QSplitter(parent)
{
    setupUi();
}

void ClipEditor::setupUi()
{
    setOrientation(Qt::Vertical);

    addWidget(clip_sub_split = new QSplitter);
    addWidget(timeline_view = new TimelineView(this));
    clip_sub_split->addWidget(preview = new FrameView);
    clip_sub_split->addWidget(new QWidget);
}
