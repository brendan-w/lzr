
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "../clip.h"

class ClipEditor : public QWidget
{
    Q_OBJECT

public:
    ClipEditor(QWidget* parent = 0);
    void setModel(Clip* clip);

private:
    void setupUi();

    //GUI
    QHBoxLayout* hbox;

    //data
    Clip* model;
};
