
#pragma once

#include <QStyledItemDelegate>

#define PATH_DELEGATE_SIZE 50


class PathDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    PathDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const Q_DECL_OVERRIDE;
    //QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex& index) const Q_DECL_OVERRIDE;
    //void setEditorData(QWidget *editor, const QModelIndex& index) const Q_DECL_OVERRIDE;
    //void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex& index) const Q_DECL_OVERRIDE;

private slots:
    //void commitAndCloseEditor();
};
