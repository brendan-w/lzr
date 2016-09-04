
#pragma once

#include <QtWidgets>
#include <QDebug>
#include <liblzr.h>
#include "../frameview.h"
#include "curveview.h"

class ClipEditor : public QSplitter
{
    Q_OBJECT

public:
    ClipEditor(QWidget* parent = 0);

private:
    void setupUi();

    QSplitter* clip_sub_split;

    FrameView* preview;
    CurveView* curve_view;
};
