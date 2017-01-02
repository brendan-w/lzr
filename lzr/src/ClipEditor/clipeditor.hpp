
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "../clip.hpp"

class ClipEditor : public QWidget
{
    Q_OBJECT

public:
    ClipEditor(QWidget* parent = 0);
    void setModel(Clip* clip);

private:
    void setupUi();

    //GUI
    QVBoxLayout* vbox;
    QScrollArea* effects_scroll;
    QVBoxLayout* effects;

    //data
    Clip* model;
};
