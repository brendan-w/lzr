
#include "clipeditor.h"


ClipEditor::ClipEditor(QWidget* parent) : QWidget(parent)
{
    setupUi();
}

void ClipEditor::setupUi()
{
    setLayout(hbox = new QHBoxLayout(this));
}

void ClipEditor::setModel(Clip* clip)
{
    model = clip;
    //TODO: connect signals
}
