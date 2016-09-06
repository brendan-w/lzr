
#include "effectitem.h"


EffectItem::EffectItem(Effect* e, QWidget* parent) : QWidget(parent), effect(e)
{
    setLayout(grid = new QGridLayout(this));

    //the effect's name
    grid->addWidget(name = new QLabel(effect->name, this), 0, 0, 1, 3);
    QFont font = name->font();
    //font.setPointSize(10);
    font.setBold(true);
    name->setFont(font);

    //add lines for each parameter
    int row = 1;
    for(QString param : effect->params.keys())
    {
        QLabel* label = new QLabel(param, this);
        QComboBox* signal_select = new QComboBox(this);

        for(Signal* s : effect->params[param]->sigs)
        {
            signal_select->addItem(s->name);
        }

        label->setMaximumWidth(100);
        label->setContentsMargins(0, 3, 0, 3);
        signal_select->setMaximumWidth(100);

        grid->addWidget(label, row, 0, Qt::AlignTop);
        grid->addWidget(signal_select, row, 1, Qt::AlignTop);

        QPushButton* b = new QPushButton("asdf", this);
        b->setMinimumHeight(100);
        grid->addWidget(b, row, 2, Qt::AlignTop);
        row++;
    }
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
