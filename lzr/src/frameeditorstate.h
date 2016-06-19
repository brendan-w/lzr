
#pragma once

enum tool_t {
    POINTER,
    DRAW,
    INSERT,
    DELETE
};

typedef struct {
    tool_t tool;
    QColor color;
    int grid_divisions;
    bool snap; //momentary hotkeys
    bool reverse;
} FrameEditorState;
