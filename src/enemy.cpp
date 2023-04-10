#include "enemy.h"

void tick_enemies(std::vector<Enemy>& enemies, King& king, std::vector<Tile>& tiles, SurfaceMap& surface_map, std::vector<Emote>& emotes, Sequences& sequences, Sounds& sounds, Settings& settings, Platform& platform, double delta_time)
{
	const double patrol_speed = 50; // TODO: Settings!
	const double edge_tolerance = 0;
	const double edge_pause_length = 1;
	const double lost_king_pause_length = 0.6;
	const double seen_freeze_length = 0.3;

	Surface* king_surface = surface_from_entity_position(king.position, tiles, surface_map);

	for(Enemy& enemy : enemies) {
		Surface* enemy_surface = surface_from_entity_position(enemy.position, tiles, surface_map);
		if(enemy_surface == nullptr) {
			enemy.state = EnemyState::LOST;
		}

		enemy.visual_y_velocity += settings.gravity * 1.3 * delta_time;
		enemy.visual_y_offset += enemy.visual_y_velocity * delta_time;
		if(enemy.visual_y_offset >= 0) {
			enemy.visual_y_offset = 0;
		}

		bool seen_left = (enemy.velocity.x > 0 && enemy.position.x < king.position.x);
		bool seen_right = (enemy.velocity.x < 0 && enemy.position.x > king.position.x);
		bool seen_king = (enemy_surface == king_surface && (seen_left || seen_right));

		// PATROL STATE
		if(enemy.state == EnemyState::PATROL) {
			// Move if stopped (from PAUSE state)
			if(epsilon_equals(enemy.velocity.x, 0)) {
				enemy.velocity.x = patrol_speed * enemy.stored_x_direction;
			}

			if(seen_king) {
				enemy.visual_y_velocity = -180; // TODO: settings
				buffer_sound(platform, sounds.enemy_seen_king, 1);
				activate_emote(enemy.emote, &sequences.emote_alarm, 1);
				enemy.state = EnemyState::SEENFREEZE;
				enemy.time_to_unpause = seen_freeze_length;
				if (enemy.velocity.x < 0) {
					enemy.stored_x_direction = -1;
				}
				else if (enemy.velocity.x > 0) {
					enemy.stored_x_direction = 1;
				}
				enemy.velocity.x = 0;
			}
			
			// Check if at edges
			bool is_at_edge = false;
			if(enemy.velocity.x < 0 && enemy.position.x < tiles[enemy_surface->left_edge_tile].position.x - edge_tolerance) {
				is_at_edge = true;
				enemy.stored_x_direction = 1;
			} else if(enemy.velocity.x > 0 && enemy.position.x > tiles[enemy_surface->right_edge_tile].position.x + edge_tolerance) {
				is_at_edge = true;
				enemy.stored_x_direction = -1;
			}

			if(is_at_edge) {
				enemy.velocity.x = 0;
				enemy.state = EnemyState::PAUSE;
				enemy.time_to_unpause = edge_pause_length;
			}
		} else if(enemy.state == EnemyState::CHASE) {
			// TODO: refactor edge check logic for here. As is, enemies can walk over gaps created between seeing the king and now
			if(!seen_king) {
				enemy.velocity.x = 0;
				enemy.state = EnemyState::PAUSE;
				enemy.time_to_unpause = lost_king_pause_length;
				buffer_sound(platform, sounds.enemy_lost_king, 1);
				activate_emote(enemy.emote, &sequences.emote_confused, 1);
			}
		} else if(enemy.state == EnemyState::PAUSE) {
			enemy.time_to_unpause -= delta_time;
			if(enemy.time_to_unpause <= 0) {
				enemy.state = EnemyState::PATROL;
			}
		}
		else if (enemy.state == EnemyState::SEENFREEZE) {
			enemy.time_to_unpause -= delta_time;
			if (enemy.time_to_unpause <= 0) {
				enemy.state = EnemyState::CHASE;
				enemy.velocity.x = enemy.stored_x_direction * patrol_speed * 1.75; // Mod settings!
			}
		}

		enemy.position.x += enemy.velocity.x * delta_time;

		enemy.animator.frame_length = 0.08;
		if(seen_king) {
			enemy.animator.frame_length = 0.06;
		}
		enemy.animator.sequence = &sequences.guard_idle;
		if (enemy.velocity.x != 0) {
			enemy.animator.sequence = &sequences.guard_run;
			if (enemy.velocity.x < 0) {
				enemy.animator.is_flipped = true;
			} else {
				enemy.animator.is_flipped = false;
			}
		}
		tick_animator(enemy.animator, delta_time);
		tick_emote(enemy.emote, delta_time);
	}
}

bool is_king_caught(std::vector<Enemy>& enemies, King& king)
{
	for(Enemy& enemy : enemies) {

		if(is_colliding(enemy.collider, enemy.position, king.collider, king.position)) {
			return true;
		}
	}

	return false;
}

Surface* surface_from_entity_position(Vec2& position, std::vector<Tile>& tiles, SurfaceMap& surface_map)
{
	const double x_tolerance = 12; // TODO: Settings
	const double y_tolerance = 12; // TODO: Settings

	Vec2 projected_position = Vec2(position.x, position.y + 16);
	for(int i = 0; i < tiles.size(); ++i) {
		if(epsilon_equals(projected_position.x, tiles[i].position.x, x_tolerance)
		&& epsilon_equals(projected_position.y, tiles[i].position.y, y_tolerance)) {
			return &surface_map.surfaces[surface_map.tile_surface_indices[i]];
		}
	}

	return nullptr;
}

double get_stored_x_direction(Enemy& enemy) {
	if (enemy.velocity.x < 0) {
		return 1;
	}
	else if (enemy.velocity.x > 0) {
		return -1;
	}
	return 0; // TODO: should we?
}