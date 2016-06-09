
#include <QPainter>
#include "pathdelegate.h"
#include "frame.h" //lzr::frame QT Metatype
#include <QDebug>


void PathDelegate::paint(QPainter* painter,
                         const QStyleOptionViewItem& option,
                         const QModelIndex& index) const
{
    if(option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    int WIDTH = option.rect.width();

    painter->save();

    painter->translate(option.rect.x(), option.rect.y());
    painter->fillRect(0, 0, WIDTH, WIDTH, Qt::black);

    WIDTH--; //compensate for a OBO pxiel issue with drawing at the edges
    painter->scale((WIDTH/2), -(WIDTH/2));
    painter->translate(1, -1);

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

    painter->restore();
}

QSize PathDelegate::sizeHint(const QStyleOptionViewItem& option,
                             const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(PATH_DELEGATE_SIZE,
                 PATH_DELEGATE_SIZE);
}
