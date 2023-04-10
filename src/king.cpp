#include "king.h"

void tick_king(King& king, Platform& platform, Sequences& sequences, Sounds& sounds, const Settings& settings, double delta_time)
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
    double same_direction_mod = 1; // TODO: settings
    double modded_acceleration = acceleration * king.acceleration_mod;
	if (platform.input.left.held) {
        if(king.jump_state == JumpState::GROUND && king.velocity.x <= 0) {
            modded_acceleration *= same_direction_mod;
        }
        king.animator.is_flipped = true;
		king.velocity.x -= modded_acceleration * delta_time;
		if(king.velocity.x < -max_speed) {
			king.velocity.x = -max_speed;
		}
	}

	if(platform.input.right.held) {
        if(king.jump_state == JumpState::GROUND && king.velocity.x >= 0) {
            modded_acceleration *= same_direction_mod;
        }
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
    tick_animator(king.animator, delta_time);
}

bool is_king_dead(King& king, Platform& platform)
{
    if(king.position.y >= platform.logical_height) {
        return true;
    }
    return false;
}

void resolve_king_velocity(King& king, std::vector<Tile>& tiles, Sounds& sounds, Platform& platform, double delta_time)
{
	Rect king_col = offset_collider(king.collider, king.position);
	king.is_grounded = false;
    
    bool is_tiles_changed = false;
	for(Tile& tile : tiles) {
		if (tile.health <= 0){
			continue;
		}
		Rect tile_col = offset_collider(tile.collider, tile.position);

		Rect king_post_x_vel = king_col;
		king_post_x_vel.position.x += king.velocity.x * delta_time;
		if(is_colliding(king_post_x_vel, tile_col)) {
			king.velocity.x = 0;
		}

		Rect king_post_y_vel = king_col;
		king_post_y_vel.position.y += king.velocity.y * delta_time;
		if(is_colliding(king_post_y_vel, tile_col)) {
			if (king.velocity.y > 0 && !tile.is_crumbling) {
				tile.is_crumbling = true;
				tile.time_till_crumble = 0.5; // TODO: Settings for crumble length
                tile.visible_health--;
                buffer_sound(platform, sounds.tile_crumbles[random_int(sounds.tile_crumbles.size())], 1);
                is_tiles_changed = true;
			}
			king.velocity.y = 0;
		}

		Rect ground_check_rect = king_col;
		ground_check_rect.position.y += 0.75;
		if(king.velocity.y >= 0 && is_colliding(ground_check_rect, tile_col)) {
			king.is_grounded = true;
		}
	}

	king.position.x += king.velocity.x * delta_time;
	king.position.y += king.velocity.y * delta_time;

    if (is_tiles_changed) {
        update_tile_orientation(tiles);
    }
}

