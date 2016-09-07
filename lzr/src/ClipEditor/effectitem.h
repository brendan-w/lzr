
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "../effects.h"



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

private:
    //data
    Effect* effect;
};
