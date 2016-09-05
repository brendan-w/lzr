
#pragma once

#include <liblzr.h>
#include <QAbstractListModel>
#include <QColor>
#include "effects.h"



class Clip : public QAbstractListModel
{
    Q_OBJECT

public:
    Clip();
    ~Clip();
    lzr::Frame run(Time& t);

private:
    QList<Effect*> effects;
};
