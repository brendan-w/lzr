
#pragma once

#include <QtWidgets>
#include <QDebug>


class SettingsDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit SettingsDock(QWidget* parent = 0);

signals:
    void grid_changed(int n);
    void draw_density_changed(double d);

private:
    QWidget* content;
    QVBoxLayout* layout;

    QSpinBox* grid_divisions;
    QDoubleSpinBox* draw_density;
};
