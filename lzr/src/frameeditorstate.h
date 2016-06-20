
#pragma once

enum tool_t {
    POINTER,
    DRAW,
    INSERT,
    DELETE,
    BRUSH
};

typedef struct {
    tool_t tool;
    QColor color;
    double brush_size;
    int grid_divisions;
    double draw_density;
    bool snap; //momentary hotkeys
    bool reverse;
} FrameEditorState;
