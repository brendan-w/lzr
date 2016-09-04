
#pragma once

#include <QtWidgets>
#include <liblzr.h>

class FrameView : public QWidget
{
    Q_OBJECT

public:
    FrameView(QWidget* parent = 0);
    void setFrame(lzr::Frame f);
    QSize sizeHint() const;
    QSizePolicy sizePolicy() const;

protected:
    void paintEvent(QPaintEvent* event);

private:
    lzr::Frame frame;
};
