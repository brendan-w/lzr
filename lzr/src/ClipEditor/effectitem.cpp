
#include "signaldisplays.h"
#include "effectitem.h"


EffectItem::EffectItem(Effect* e, QWidget* parent) : QWidget(parent), effect(e)
{
    setLayout(grid = new QGridLayout(this));

    //the effect's name
    QLabel* name = new QLabel(effect->name, this);
    grid->addWidget(name, 0, 0, 1, 3);
    QFont font = name->font();
    //font.setPointSize(10);
    font.setBold(true);
    name->setFont(font);

    //add lines for each parameter
    int row = 1;
    for(QString param_name : effect->params.keys())
    {
        Param* param = effect->params[param_name];
        QLabel* label = new QLabel(param_name, this);
        QComboBox* combo = new QComboBox(this);

        //zero out the signal widget map
        params[param_name] = {
            .param = param,
            .combo = combo,
            .signal = NULL,
            .row = row
        };

        //load the combobox
        for(SignalType type : param->sigs.keys())
        {
            combo->addItem(nameForSignal(type), type);
        }

        label->setFixedWidth(100);
        label->setContentsMargins(0, 3, 0, 3);
        combo->setFixedWidth(100);
        combo->setObjectName(param_name);

        grid->addWidget(label, row, 0, Qt::AlignTop);
        grid->addWidget(combo, row, 1, Qt::AlignTop);

        //listen for changes in signal type
        connect(combo, SIGNAL(currentIndexChanged(int)),
                this, SLOT(signalTypeChanged(int)));

        //set the current signal type
        combo->setCurrentIndex(combo->findData(param->type));
        QWidget* signal_widget = signalForParam(param);
        grid->addWidget(signal_widget, row, 2);
        params[param_name].signal = signal_widget;

        row++;
    }
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
    Q_UNUSED(index);

    //look up the effect parameter based on the ComboBox's name
    QComboBox* combo = (QComboBox*) sender();
    ParamItem* param_item = &(params[combo->objectName()]);
    Param* param = param_item->param;
    param->select((SignalType) combo->currentData().toInt());

    //kill any old signal widgets
    if(param_item->signal)
    {
        delete param_item->signal;
        param_item->signal = NULL;
    }

    //instantiate the correct widget for this signal
    QWidget* signal_widget = signalForParam(param);
    grid->addWidget(signal_widget, param_item->row, 2);
    param_item->signal = signal_widget;
}

QWidget* EffectItem::signalForParam(Param* param)
{
    switch(param->type)
    {
    case CONSTANT:
        return new ConstantSignalDisplay(param->signal(), this);
    default:
        return new QWidget(this);
    }
}

QString EffectItem::nameForSignal(SignalType type)
{
    switch(type)
    {
    case CONSTANT: return "Constant";
    case CURVE:    return "Curve";
    default:       return "<unknown>";
    }
}
