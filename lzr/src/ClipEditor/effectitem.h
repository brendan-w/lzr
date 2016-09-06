
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "../effects.h"

class EffectItem : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *e);

public:
    EffectItem(Effect* e, QWidget* parent = 0);
    QSize sizeHint() const;

private:
    Effect* effect;
};
