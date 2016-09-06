
#include "clipeditor.h"
#include "effectitem.h"


ClipEditor::ClipEditor(QWidget* parent) : QWidget(parent)
{
    setupUi();
}

void ClipEditor::setupUi()
{
    setLayout(vbox = new QVBoxLayout(this));

    vbox->addWidget(new QWidget()); //TODO: write header
    vbox->addWidget(effects_scroll = new QScrollArea(this));

    effects_scroll->setObjectName("effects_scroll");
    effects_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    effects_scroll->setFrameShadow(QFrame::Plain);
    effects_scroll->setFrameShape(QFrame::NoFrame);
    effects_scroll->setWidget(effects_content = new QWidget(effects_scroll));
    effects_scroll->setWidgetResizable(true);
    effects_scroll->setContentsMargins(0, 0, 0, 0);

    effects_content->setObjectName("effects_content");
    effects_content->setLayout(effects_vbox = new QVBoxLayout());
    // effects_content->setSizePolicy(QSizePolicy::MinimumExpanding,
                                   // QSizePolicy::MinimumExpanding);

    effects_vbox->setContentsMargins(0, 0, 0, 0);
    effects_vbox->setSpacing(2);
}

void ClipEditor::setModel(Clip* clip)
{
    model = clip;

    for(Effect* e : clip->effects)
    {
        effects_vbox->addWidget(new EffectItem(e, effects_content));
    }

    effects_vbox->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
    //TODO: connect signals
}
