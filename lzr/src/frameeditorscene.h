
#pragma once

#include <QtWidgets>
#include <QDebug>

#include "frame.h"
#include "grid.h"
#include "path.h"


enum tool_t {
    MOVE,
    LINE,
    DRAW,
    ADD,
    DEL
};


/*
 * Editor for Frame models
 */
class FrameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    FrameScene(QWidget* parent = 0);
    void setModel(Frame* m, QItemSelectionModel* path_sel);

public slots:
    void path_selection_changed(const QItemSelection& selected,
                                const QItemSelection& deselected);
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
    //data
    Frame* model;
    QItemSelectionModel* path_selection;

    //gui
    Grid* grid;
    QList<Path*> paths;

    //controls
    QPointF mouse;
    bool reverse; //momentary hotkey
    QColor color;
    tool_t tool;

    Path* current_path();
};
