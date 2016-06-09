
#include <QPainter>
#include "pathdelegate.h"
#include "frame.h" //lzr::frame QT Metatype

#define SIZE 100

void PathDelegate::paint(QPainter* painter,
                         const QStyleOptionViewItem& option,
                         const QModelIndex& index) const
{
    Q_UNUSED(option);

    painter->scale(SIZE, -SIZE);
    painter->translate(0.5, -0.5);

    //get the raw point data from the model index
    lzr::Frame path = index.data().value<lzr::Frame>();

    for(size_t i = 0; i < path.size() - 1; i++)
    {
        lzr::Point& p1 = path[i];
        lzr::Point& p2 = path[i+1];

        QLineF line(p1.x, p1.y, p2.x, p2.y);

        //TODO: double check that color is actually
        //      a property of the second point
        QPen pen(QColor(p2.r, p2.g, p2.b, p2.i));
        pen.setCosmetic(true);
        painter->setPen(pen);
        painter->drawLine(line);
    }
}

QSize PathDelegate::sizeHint(const QStyleOptionViewItem& option,
                             const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(SIZE, SIZE);
}
