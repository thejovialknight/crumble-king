#pragma once
#include "platform.h"
#include "settings.h"
#include "utils.h"
#include "animation.h"
#include "sound.h"
#include "rect.h"
#include "emote.h"
#include "collision.h"
#include "tiles.h"

enum class JumpState {
    GROUND,
    JUMP,
    FLOAT,
    POST_FLOAT
};

struct King {
    JumpState jump_state = JumpState::GROUND;
    Vec2 position = Vec2(0, 0);
    Vec2 velocity = Vec2(0, 0);
    double jump_buffer = 0;
    double coyote_time = 0;
    double gravity_scale = 1;
    double acceleration_mod = 1;
    bool is_grounded = false;
    bool is_facing_right = true;
    Animator animator;
    Rect collider = Rect(-6, -16, 14, 16);
};

void tick_king(King& king, Platform& platform, Sequences& sequences, Sounds& sounds, const Settings& settings, double delta_time);
bool is_king_dead(King& king, Platform& platform);
void resolve_king_velocity(King& king, std::vector<Tile>& tiles, Sounds& sounds, Platform& platform, double delta_time); 
