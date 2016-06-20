
#include "frameview.h"
#include "utils.h"


FrameView::FrameView(QWidget* parent) : QWidget(parent)
{

}

void FrameView::setFrame(lzr::Frame f)
{
    frame = f;
    update();
}

void FrameView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int HALF_WIDTH = width() / 2;
    painter.scale(HALF_WIDTH, -HALF_WIDTH);
    painter.translate(1, -1);

    painter.fillRect(frame_bounds, Qt::black);

    for(int i = 0; i < (int) frame.size() - 1; i++)
    {
        lzr::Point& p1 = frame[i];
        lzr::Point& p2 = frame[i+1];

        QLineF line(p1.x, p1.y, p2.x, p2.y);

        //TODO: double check that color is actually
        //      a property of the second point
        QPen pen(QColor(p2.r, p2.g, p2.b, p2.i));
        pen.setCosmetic(true);
        painter.setPen(pen);
        painter.drawLine(line);
    }
}
