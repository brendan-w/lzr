
#include "clipeditor.hpp"
#include "effectitem.hpp"


ClipEditor::ClipEditor(QWidget* parent) : QWidget(parent)
{
    setupUi();
}

void ClipEditor::setupUi()
{
    setLayout(vbox = new QVBoxLayout(this));

    vbox->addWidget(new QWidget()); //TODO: write header
    vbox->addWidget(effects_scroll = new QScrollArea(this));

    QWidget* content = new QWidget(effects_scroll);
    effects_scroll->setWidget(content);
    effects_scroll->setObjectName("effects_scroll");
    effects_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    effects_scroll->setFrameShadow(QFrame::Plain);
    effects_scroll->setFrameShape(QFrame::NoFrame);
    effects_scroll->setWidgetResizable(true);
    effects_scroll->setContentsMargins(0, 0, 0, 0);

    content->setLayout(effects = new QVBoxLayout());
    content->setObjectName("effects_content");
    // content->setSizePolicy(QSizePolicy::MinimumExpanding,
                           // QSizePolicy::MinimumExpanding);

    effects->setContentsMargins(0, 0, 0, 0);
    effects->setSpacing(3);
}

void ClipEditor::setModel(Clip* clip)
{
    model = clip;

    for(Effect* e : clip->effects)
    {
        effects->addWidget(new EffectItem(e, effects_scroll));
    }

    effects->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
    //TODO: connect signals
}
