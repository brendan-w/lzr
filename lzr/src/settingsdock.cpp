
#include "settingsdock.h"


SettingsDock::SettingsDock(QWidget* parent) : QDockWidget(parent)
{
    setWindowTitle("Settings");
    setFeatures(QDockWidget::DockWidgetMovable);

    setWidget(content = new QWidget());
    layout = new QVBoxLayout(content);

    layout->addWidget(grid_divisions = new QSpinBox(content));
    grid_divisions->setRange(1, 120); //TODO: somewhat arbitrary, choose a better value
    grid_divisions->setValue(8);

    grid_divisions->setFocusPolicy(Qt::NoFocus);
    grid_divisions->setFrame(false);

    connect(grid_divisions, SIGNAL(valueChanged(int)),
            this, SIGNAL(grid_changed(int)));
}
