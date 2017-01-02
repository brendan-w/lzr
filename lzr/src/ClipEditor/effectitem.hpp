
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "../effects.hpp"



typedef struct {
    Param* param;
    QComboBox* combo;
    QWidget* signal;
    int row;
} ParamItem;

/*
 * A single effect in the list, containing parameters
 */
class EffectItem : public QWidget
{
    Q_OBJECT

public:
    EffectItem(Effect* e, QWidget* parent = 0);

public slots:
    void signalTypeChanged(int index);

protected:
    void paintEvent(QPaintEvent *e);

private:
    QWidget* signalForParam(Param* param);
    QString nameForSignal(SignalType type);

    //data
    Effect* effect;

    QGridLayout* grid;
    QMap<QString, ParamItem> params;
};
