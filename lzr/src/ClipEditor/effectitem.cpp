
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
        EffectParam* ep = effect->params[param];
        QLabel* label = new QLabel(param, this);
        QComboBox* combo = new QComboBox(this);

        //zero out the signal widget map
        params[param] = {
            .param = ep,
            .combo = combo,
            .signal = NULL,
            .row = row
        };

        //load the combobox
        for(Signal* s : ep->sigs)
        {
            combo->addItem(s->name);
        }

        //listen for changes in signal type
        connect(combo, SIGNAL(currentIndexChanged(int)),
                this, SLOT(signalTypeChanged(int)));

        label->setMaximumWidth(100);
        label->setContentsMargins(0, 3, 0, 3);
        combo->setMaximumWidth(100);
        combo->setObjectName(param);

        grid->addWidget(label, row, 0, Qt::AlignTop);
        grid->addWidget(combo, row, 1, Qt::AlignTop);

        //trigger the creation of the signal widget
        combo->setCurrentIndex(ep->s);

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

void EffectItem::signalTypeChanged(int index)
{
    //look up the effect parameter based on the ComboBox's name
    QString name = sender()->objectName();
    EffectParamItem* param = &(params[name]);
    param->param->s = index;

    //kill any old signal widgets
    if(param->signal)
    {
        delete param->signal;
        param->signal = NULL;
    }

    //instantiate the correct widget for this signal
    QWidget* signal = signalForParam(name);
    grid->addWidget(signal, param->row, 2, Qt::AlignTop);
    param->signal = signal;
}

QWidget* EffectItem::signalForParam(QString param)
{
    QWidget* w;

    if(param == "Constant")
        w = new QWidget(this);
    else if(param == "Curve")
        w = new QWidget(this);
    else
        w = new QWidget(this);

    w->setMinimumHeight(100);
    return w;
}
