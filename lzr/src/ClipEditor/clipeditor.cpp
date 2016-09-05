
#include "clipeditor.h"


ClipEditor::ClipEditor(QWidget* parent) : QWidget(parent)
{
    setupUi();
}

void ClipEditor::setupUi()
{
    setLayout(vbox = new QVBoxLayout(this));

    vbox->addWidget(new QWidget()); //TODO: write header
    vbox->addWidget(effects = new QListView());
}

void ClipEditor::setModel(Clip* clip)
{
    model = clip;
    effects->setModel(clip);
    //TODO: connect signals
}
