#pragma once
#include "vec2.h"
#include "platform.h"
#include "settings.h"
#include "utils.h"
#include "animation.h"
#include "sound.h"

enum class JumpState {
    GROUND,
    JUMP,
    FLOAT
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
};

void update_king(King& king, Platform& platform, Sequences& sequences, Sounds& sounds, const Settings& settings, double delta_time);
bool is_king_dead(King& king, Platform& platform);
