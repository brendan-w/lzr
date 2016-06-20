
#include "settingsdock.h"


SettingsDock::SettingsDock(QWidget* parent) : QDockWidget(parent)
{
    setWindowTitle("Settings");
    setFeatures(QDockWidget::DockWidgetMovable);

    setWidget(content = new QWidget());
    layout = new QVBoxLayout(content);

    QLabel* label;

    //make controls
    layout->addWidget(label = new QLabel("Grid Size", content));
    layout->setAlignment(label, Qt::AlignBottom);
    layout->addWidget(grid_divisions = new QSpinBox(content));
    grid_divisions->setRange(1, 120); //TODO: somewhat arbitrary, choose a better value
    grid_divisions->setFocusPolicy(Qt::NoFocus);

    layout->addWidget(label = new QLabel("Draw density", content));
    layout->setAlignment(label, Qt::AlignBottom);
    layout->addWidget(draw_density = new QDoubleSpinBox(content));
    draw_density->setRange(0.0, 2.0);
    draw_density->setSingleStep(0.01);
    draw_density->setFocusPolicy(Qt::NoFocus);

    //default values
    grid_divisions->setValue(8);
    draw_density->setValue(0.05);

    connect(grid_divisions, SIGNAL(valueChanged(int)),
            this, SIGNAL(grid_changed(int)));
    connect(draw_density, SIGNAL(valueChanged(double)),
            this, SIGNAL(draw_density_changed(double)));
}
