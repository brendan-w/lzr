
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "../effects.h"



typedef struct {
    EffectParam* param;
    QComboBox* combo;
    QWidget* signal;
    int row;
} EffectParamItem;


/*
 * A single effect in the list, containing parameters
 */
class EffectItem : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *e);

public:
    EffectItem(Effect* e, QWidget* parent = 0);
    QSize sizeHint() const;

private slots:
    void signalTypeChanged(int index);

private:
    QWidget* signalForParam(QString param);

    //data
    Effect* effect;

    //GUI
    QGridLayout* grid;
    QLabel* name;
    QMap<QString, EffectParamItem> params; //param name --> EffectParamItem
};
