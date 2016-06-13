
#pragma once

enum tool_t {
    MOVE,
    LINE,
    DRAW,
    ADD,
    DEL
};

typedef struct {
    Grid* grid;
    bool reverse; //momentary hotkey
    QColor color;
    tool_t tool;
} FrameEditorState;
