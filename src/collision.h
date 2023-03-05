#pragma once
#include <vector>
#include "rect.h"
#include "platform.h"
#include "random.h"

// Returns whether two rects are overlapping (colliding).
bool is_colliding(Rect& a, Rect& b);
bool is_colliding(Rect& a_col, Vec2& a_pos, Rect& b_col, Vec2& b_pos);
Rect offset_collider(Rect& collider, Vec2& position);
