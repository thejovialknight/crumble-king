#include "king.h"

void update_king(King& king, Platform& platform, Sequences& sequences, Sounds& sounds, const Settings& settings, double delta_time)
{
    const double max_speed = settings.king_max_speed;
    const double acceleration = settings.king_acceleration;
    const double jump_speed = settings.king_jump_speed;
    const double jump_buffer_length = settings.jump_buffer_length;
    const double coyote_time_length = settings.coyote_time_length;
    const double float_velocity = settings.float_velocity;
    const double float_start_gravity_scale = settings.float_start_gravity_scale;
    const double float_target_gravity_scale = settings.float_target_gravity_scale;
    const double float_gravity_lerp_speed = settings.float_gravity_lerp_speed;
    const double fall_gravity_scale = settings.fall_gravity_scale;
    const double gravity = settings.gravity;

    // Gravity
    king.velocity.y += gravity * king.gravity_scale * delta_time;

    // Horizontal movement:
    double modded_acceleration = acceleration * king.acceleration_mod;
	if (platform.input.left.held) {
        king.animator.is_flipped = true;
		king.velocity.x -= modded_acceleration * delta_time;
		if(king.velocity.x < -max_speed) {
			king.velocity.x = -max_speed;
		}
	}

	if(platform.input.right.held) {
        king.animator.is_flipped = false;
		king.velocity.x += modded_acceleration * delta_time;

		if(king.velocity.x > max_speed) {
			king.velocity.x = max_speed;
		}
	}

    king.jump_buffer -= delta_time;
    king.coyote_time -= delta_time;

    if(king.is_grounded) {
        king.gravity_scale = 1;
        king.jump_state = JumpState::GROUND;

        if(king.jump_buffer > 0) {
            king.jump_state = JumpState::JUMP;
            king.velocity.y = -jump_speed;
            king.jump_buffer = 0;
            buffer_sound(platform, sounds.king_jump, 1);
        }
    }

    if(king.jump_state == JumpState::GROUND) {
        if(!king.is_grounded) {
            king.jump_state = JumpState::JUMP;
            king.coyote_time = coyote_time_length;
        }

        // Jump 
        if(platform.input.jump.just_pressed) {
            king.jump_state = JumpState::JUMP;
            king.velocity.y = -jump_speed;
            king.coyote_time = 0;
            buffer_sound(platform, sounds.king_jump, 1);
        }

        if(!platform.input.left.held && !platform.input.right.held) {
            if(king.velocity.x > 0) {
                king.velocity.x -= acceleration * delta_time;
                if(king.velocity.x < 0) {
                    king.velocity.x = 0;
                }
            }

            if(king.velocity.x < 0) {
                king.velocity.x += acceleration * delta_time;
                if(king.velocity.x > 0) {
                    king.velocity.x = 0;
                }
            }
        }
    } else if(king.jump_state == JumpState::JUMP) {
        if(king.velocity.y > 0) {
            king.gravity_scale = fall_gravity_scale;
        }

        if(platform.input.jump.just_pressed) {
            if(king.coyote_time > 0) {
                king.coyote_time = 0;
                king.velocity.y = -jump_speed;
                buffer_sound(platform, sounds.king_jump, 1);
            } else {
                king.jump_state = JumpState::FLOAT;
                buffer_sound(platform, sounds.king_float, 1);
                king.velocity.y = -float_velocity;
                king.gravity_scale = float_start_gravity_scale;

                if(platform.input.left.held /* && physics.velocity.x > 0*/ ) {
                    king.velocity.x = -max_speed / 2;
                }

                if(platform.input.right.held /* && physics.velocity.x < 0 */) {
                    king.velocity.x = (max_speed / 2);
                }
            }
        }
    } else if(king.jump_state == JumpState::FLOAT) {
        king.gravity_scale = lerp(king.gravity_scale, float_target_gravity_scale, float_gravity_lerp_speed * delta_time);  

        if(platform.input.jump.just_pressed) {
            king.jump_buffer = jump_buffer_length;
        }
    }

    // Set king sprite
    king.animator.frame_length = 0.075;
    king.animator.sequence = &sequences.king_idle;

    if (king.velocity.x != 0) { king.animator.sequence = &sequences.king_run; }

    if(!king.is_grounded) { 
        king.animator.sequence = &sequences.king_jump; 
        if (king.jump_state == JumpState::FLOAT) {
            king.animator.sequence = &sequences.king_float;
        }
    }
    iterate_animator(king.animator, delta_time);
}

bool is_king_dead(King& king, Platform& platform)
{
    if(king.position.y >= platform.logical_height) {
        return true;
    }
    return false;
}
