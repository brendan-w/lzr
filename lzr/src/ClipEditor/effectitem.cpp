
#include "effectitem.h"


EffectItem::EffectItem(Effect* e, QWidget* parent) : QWidget(parent), effect(e)
{
    setLayout(hbox = new QHBoxLayout(this));

    hbox->addLayout(main = new QVBoxLayout(this));

    //the effect's name
    main->addWidget(name = new QLabel(effect->name, this));
    QFont font = name->font();
    font.setPointSize(12);
    font.setBold(true);
    name->setFont(font);

    //add lines for each parameter
    for(QString param : effect->params.keys())
    {
        main->addWidget(new QLabel(param, this));
    }

    hbox->addWidget(new QPushButton("asdf", this));
}

QSize EffectItem::sizeHint() const
{
    return QSize(100, 100);
}

void EffectItem::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    //just to get stylesheets to work
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
