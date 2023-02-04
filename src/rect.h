#pragma once
#include "vec2.h"

// A position and a size, represents a rectangular shape.
struct Rect {
    Vec2 position = Vec2(0, 0); // Top left position of the rect.
    Vec2 size = Vec2(0, 0); // The size of the rect. position + size = bottom right position of the rect.

    Rect() {}
    Rect(Vec2 pos, Vec2 siz) : position(pos), size(siz) {}
    Rect(double x, double y, double width, double height) : position(Vec2(x, y)), size(Vec2(width, height)) {}
};

struct IRect {
    Vec2 position = Vec2(0, 0); // Top left position of the rect.
    Vec2 size = Vec2(0, 0); // The size of the rect. position + size = bottom right position of the rect.

    IRect() {}
    IRect(Vec2 pos, Vec2 siz) : position(pos), size(siz) {}
    IRect(int x, int y, int width, int height) : position(Vec2(x, y)), size(Vec2(width, height)) {}
};