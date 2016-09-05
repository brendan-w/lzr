
#pragma once

#include <QtWidgets>
#include <QDebug>

class ClipEditor : public QWidget
{
    Q_OBJECT

public:
    ClipEditor(QWidget* parent = 0);

private:
    void setupUi();

    QHBoxLayout* hbox;
};
