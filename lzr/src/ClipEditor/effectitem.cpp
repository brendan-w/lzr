
#include "effectitem.h"


EffectItem::EffectItem(Effect* e, QWidget* parent) : QWidget(parent), effect(e)
{
    new QPushButton("asdf", this);
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
