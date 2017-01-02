
#include "frameview.hpp"
#include "utils.hpp"


FrameView::FrameView(QWidget* parent) : QWidget(parent)
{

}

QSize FrameView::sizeHint() const
{
    return QSize(200, 200);
}

QSizePolicy FrameView::sizePolicy() const
{
    return QSizePolicy(QSizePolicy::Expanding,
                       QSizePolicy::Expanding);
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
