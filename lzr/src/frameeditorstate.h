
#pragma once

enum tool_t {
    MOVE,
    LINE,
    DRAW,
    ADD,
    DEL
};

typedef struct {
    tool_t tool;
    QColor color;
    int grid_divisions;
    bool snap; //momentary hotkeys
    bool reverse;
} FrameEditorState;
