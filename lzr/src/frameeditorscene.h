
#pragma once

#include <QtWidgets>
#include <QDebug>

#include "frame.h"
#include "grid.h"
#include "path.h"
#include "frameeditorstate.h"

/*
 * Editor for Frame models
 */
class FrameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    FrameScene(QWidget* parent = 0);
    ~FrameScene();
    void setModel(Frame* m, QItemSelectionModel* path_sel);

public slots:
    void model_changed(const QModelIndex& first,
                       const QModelIndex& last);
    void path_selection_changed(const QItemSelection& selected,
                                const QItemSelection& deselected);
    void path_added(const QModelIndex& parent, int first, int last);
    void path_removed(const QModelIndex& parent, int first, int last);
    void path_changed(Path* p);
    void tool_changed(tool_t t);
    void color_changed(QColor c);
    void grid_changed(int divisions);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void drawForeground(QPainter* painter, const QRectF& rect);

private:
    Frame* model;
    QItemSelectionModel* path_selection;
    Grid* grid;
    QList<Path*> paths;
    QPointF mouse;
    FrameEditorState* state;

    Path* new_path(QModelIndex index);
    Path* current_path();
};
