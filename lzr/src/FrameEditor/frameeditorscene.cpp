
#include <QtGui>
#include "frameeditorscene.hpp"
#include "../utils.hpp"
#include "../settings.hpp"


FrameScene::FrameScene(QWidget *parent) : QGraphicsScene(parent)
{
    //enforce custom coordinate system [-1.0, 1.0]
    //Y is negative to make positive values go upwards
    setSceneRect(-3.0, -3.0, 6.0, 6.0); //bigger than (-1.0, -1.0, 2.0, 2.0) for scrollability
    setBackgroundBrush(Qt::black);

    state = new FrameEditorState;
    state->brush_size = 0.1;
    state->grid_divisions = 8;
    state->draw_density = 0.05;
    state->snap = false;
    state->reverse = false;

    addItem(grid = new Grid(state));
    addItem(marker = new Point(state));
    addItem(selector = new QGraphicsRectItem());

    selector->setVisible(false);
    selector->setEnabled(false);
    selector->setPen(QPen(Qt::gray, 0, Qt::DashLine));
}

FrameScene::~FrameScene()
{
    delete state;
}

void FrameScene::setModel(Frame* m, QItemSelectionModel* path_sel)
{
    foreach(Path* path, paths)
    {
        removeItem(path);
        delete path;
    }

    paths.clear();

    model = m;

    connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(model_changed(const QModelIndex&, const QModelIndex&)));
    connect(model, SIGNAL(rowsInserted(const QModelIndex&, int, int)),
            this, SLOT(path_added(const QModelIndex&, int, int)));
    connect(model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
            this, SLOT(path_removed(const QModelIndex&, int, int)));

    //create all of the path objects in the model
    for(int i = 0; i < model->rowCount(); i++)
    {
        paths.append(new_path(model->index(i)));
    }

    //the path selection model
    path_selection = path_sel;
    connect(path_selection, SIGNAL(selectionChanged(const QItemSelection&,
                                                    const QItemSelection&)),
            this, SLOT(path_selection_changed(const QItemSelection&,
                                              const QItemSelection&)));
}

Path* FrameScene::new_path(QModelIndex index)
{
    Path* path = new Path(state, index);
    addItem(path);

    connect(path, SIGNAL(changed(Path*)),
            this, SLOT(path_changed(Path*)));

    return path;
}

void FrameScene::drawForeground(QPainter* painter, const QRectF& rect)
{
    Q_UNUSED(rect); //because we always render the entire scene

    //always keep the marker set to the current color
    marker->set_color(state->color);

    //lookup the currently selected path
    Path* path = current_path();

    if(path && state->tool == DRAW)
    {
        QPointF pos = constrain_and_snap(mouse,
                                         state->snap,
                                         state->grid_divisions);

        marker->setPos(pos);
        marker->setVisible(true);

        painter->setPen(QPen(Qt::darkGray, 0));

        if(path->size() > 0)
        {
            Point* point;

            if(!state->reverse)
                point = path->last();
            else
                point = path->first();

            painter->drawLine(QLineF(point->x(), point->y(), pos.x(), pos.y()));
        }

    }
    else
    {
        if(state->tool != INSERT)
            marker->setVisible(false);
    }
}

void FrameScene::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    switch(state->tool)
    {
        case DRAW:
            if(current_path())
            {
                QPointF pos = constrain_and_snap(e->scenePos(),
                                                 state->snap,
                                                 state->grid_divisions);
                current_path()->add_point(pos,
                                          state->reverse ? PATH_START : PATH_END);
            }
            break;
        case POINTER:
            if(!clicked_on_point(e))
            {
                //if the user DIDN'T click on a point, start a selection rectangle
                QRectF rect(e->scenePos(), e->scenePos());
                selector->setRect(rect);
                selector->setVisible(true);
            }
            break;
        case INSERT:
        {
            QPointF pos;
            Path* path_picked;
            int after; //where in the path to insert the new point

            if(nearest_point_to_add(e->scenePos(), pos, path_picked, after))
            {
                path_picked->add_point(pos, after + 1);
                marker->setVisible(false);
            }
        }
            break;
        default:
            break;
    }

    QGraphicsScene::mousePressEvent(e);
}

void FrameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
    if(selector->isVisible())
    {
        select_rect(selector->rect());
        selector->setVisible(false);
        update();
    }

    QGraphicsScene::mouseReleaseEvent(e);
}

void FrameScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    mouse = e->scenePos();
    Path* path = current_path();

    switch(state->tool)
    {
        case DRAW:
            if(path &&
               (path->size() > 0) &&
               (e->buttons() & Qt::LeftButton))
            {
                QPointF prev = state->reverse ? path->first()->pos() : path->last()->pos();
                QPointF pos = constrain_and_snap(mouse,
                                                 state->snap,
                                                 state->grid_divisions);
                float dist = distance_between_points(prev, pos);

                if(dist >= state->draw_density)
                    current_path()->add_point(pos,
                                              state->reverse ? PATH_START : PATH_END);
            }
            update(); //keep the screen updated during drawing
            break;
        case POINTER:
            if(selector->isVisible())
            {
                //if we're in selecting with the box, keep the box size updated
                QRectF rect = selector->rect();
                rect.setBottomRight(e->scenePos());
                selector->setRect(rect);
            }
            break;
        case INSERT:
        {
            QPointF point;
            Path* path_picked;
            int after; //where in the path to insert the new point

            if(nearest_point_to_add(mouse, point, path_picked, after))
            {
                marker->setPos(point);
                marker->setVisible(true);
            }
            else
            {
                marker->setVisible(false);
            }
        }
            break;
        default:
            break;
    }

    QGraphicsScene::mouseMoveEvent(e);
}

void FrameScene::keyPressEvent(QKeyEvent* e)
{
    if(!e->isAutoRepeat())
    {
        switch(e->key())
        {
            case EDITOR_SNAP_KEY:
                state->snap = true;
                update();
                break;
            case EDITOR_REVERSE_KEY:
                state->reverse = true;
                update();
                break;
            case EDITOR_DELETE_KEY:
                foreach(QGraphicsItem* item, selectedItems())
                {
                    ((Point*) item)->remove();
                }
                update();
                break;
            case EDITOR_ADD_PATH_KEY:
                //stow the current path selection
                saved_selection = path_selection->selection();
                saved_current = path_selection->currentIndex();
                path_selection->setCurrentIndex(model->add_path(),
                                                QItemSelectionModel::ClearAndSelect);
                break;
        }
    }

    QGraphicsScene::keyPressEvent(e);
}

void FrameScene::keyReleaseEvent(QKeyEvent* e)
{
    if(!e->isAutoRepeat())
    {
        switch(e->key())
        {
            case EDITOR_SNAP_KEY:
                state->snap = false;
                update();
                break;
            case EDITOR_REVERSE_KEY:
                state->reverse = false;
                update();
                break;
            case EDITOR_ADD_PATH_KEY:
                if(paths.back()->size() == 0)
                {
                    //remove the still-empty path
                    model->removeRow(paths.size() - 1);

                    //reinstate the old path selection
                    path_selection->select(saved_selection,
                                           QItemSelectionModel::ClearAndSelect);
                    path_selection->setCurrentIndex(saved_current,
                                                    QItemSelectionModel::Select);
                }
                break;
        }
    }

    QGraphicsScene::keyReleaseEvent(e);
}

Path* FrameScene::current_path()
{
    QModelIndex current = path_selection->currentIndex();
    if(current.isValid())
        return paths[current.row()];
    return NULL;
}

bool FrameScene::clicked_on_point(QGraphicsSceneMouseEvent* e)
{
    if(views().size() == 0)
        return false;

    QGraphicsItem* item = itemAt(e->scenePos(),
                                 views()[0]->transform());
    Point* point = qgraphicsitem_cast<Point*>(item);

    if(!point)
        return false;
    else
        return point->isEnabled();
}

void FrameScene::select_rect(QRectF rect)
{
    QPainterPath path;
    path.addRect(rect);

    if(views().size() == 0)
        return;

    setSelectionArea(path, views()[0]->transform());
}

bool FrameScene::nearest_point_to_add(const QPointF& mouse,
                                      QPointF& point, //the point we'd be adding
                                      Path*& best_path, //the path we'd add it to
                                      int& after) //where in the path to add the point
{
    best_path = NULL;
    float best_distance = 2.0;

    foreach(const QModelIndex& index, path_selection->selectedRows())
    {
        Path* path = paths[index.row()];
        for(int p = 0; p < (int) path->size() - 1; p++)
        {
            QPointF a = path->at(p)->pos();
            QPointF b = path->at(p + 1)->pos();
            QPointF i;

            if(perpendicular_intersection(a, b, mouse, i))
            {
                float dist = distance_between_points(mouse, i);

                if((dist < EDITOR_INSERT_DIST) &&
                   (dist < best_distance))
                {
                    best_path = path;
                    best_distance = dist;
                    point = i;
                    after = p;
                }
            }
        }
    }

    return (best_path != NULL);
}

/*
 * Slots
 */

void FrameScene::path_selection_changed(const QItemSelection& selected,
                                        const QItemSelection& deselected)
{
    //deselect paths
    foreach(const QModelIndex& index, deselected.indexes())
    {
        if(index.row() >= paths.size())
        {
            qDebug() << "Error: model refers to path that doesn't exist in the editor";
            continue;
        }

        Path* path = paths[index.row()];
        path->setEnabled(false);
    }

    //select paths
    foreach(const QModelIndex& index, selected.indexes())
    {
        if(index.row() >= paths.size())
        {
            qDebug() << "Error: model refers to path that doesn't exist in the editor";
            continue;
        }

        Path* path = paths[index.row()];
        path->setEnabled(true);
    }
}

void FrameScene::model_changed(const QModelIndex& first, const QModelIndex& last)
{
    for(int i = first.row(); i <= last.row(); i++)
    {
        lzr::Frame path = model->get_path(model->index(i));
        paths[i]->from_LZR(path);
    }
}

void FrameScene::path_changed(Path* path)
{
    //temporarily disconnect, to prevent the dataChanged() signal from thrashing the GUI
    //TODO: there must be a better way to do this...
    disconnect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
               this, SLOT(model_changed(const QModelIndex&, const QModelIndex&)));

    //update the model with the new path data
    model->set_path(path->get_index(),
                    path->to_LZR());

    connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(model_changed(const QModelIndex&, const QModelIndex&)));
}

void FrameScene::path_added(const QModelIndex& parent, int first, int last)
{
    Q_UNUSED(parent);

    for(int i = first; i <= last; i++)
    {
        paths.insert(i, new_path(model->index(i)));
    }
}

void FrameScene::path_removed(const QModelIndex& parent, int first, int last)
{
    Q_UNUSED(parent);

    for(int i = first; i <= last; i++)
    {
        delete paths[first];
        paths.removeAt(first);
    }
}

void FrameScene::tool_changed(tool_t t)
{
    state->tool = t;
    update();
}

void FrameScene::color_changed(QColor c)
{
    state->color = c;
}

void FrameScene::brush_size_changed(double s)
{
    state->brush_size = s;
}

void FrameScene::grid_changed(int divisions)
{
    state->grid_divisions = divisions;
    update();
}

void FrameScene::draw_density_changed(double d)
{
    state->draw_density = d;
}

void FrameScene::select_all_points()
{
    foreach(const QModelIndex& index, path_selection->selectedRows())
    {
        paths[index.row()]->select_all();
    }
}

void FrameScene::deselect_all_points()
{
    foreach(const QModelIndex& index, path_selection->selectedRows())
    {
        paths[index.row()]->deselect_all();
    }
}
