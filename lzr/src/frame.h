
#pragma once

#include "liblzr.h"
#include <QAbstractListModel>


class Frame : public QAbstractListModel
{
    Q_OBJECT

public:
    Frame(lzr::Frame frame);
};
