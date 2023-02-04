#include "collision.h"

bool is_colliding(Rect a, Rect b)
{
	if(a.position.x < b.position.x + b.size.x &&
		a.position.x + a.size.x > b.position.x &&
		a.position.y < b.position.y + b.size.y &&
		a.position.y + a.size.y > b.position.y)
	{
		return true;
	}

	return false;
}

void resolve_king_velocity(King& king, std::vector<Tile>& tiles, Sounds& sounds, Platform& platform, double delta_time)
{
	Rect player_collider(Vec2(king.position.x, king.position.y - 9), Vec2(16, 16));
	king.is_grounded = false;

	// Loop through collision checks
	for(Tile& tile : tiles) {
		if (tile.health <= 0){
			continue;
		}

		Rect tile_collider(Vec2(tile.position.x, tile.position.y), Vec2(16, 16));

		Rect player_after_x_movement = player_collider;
		player_after_x_movement.position.x += king.velocity.x * delta_time;

		Rect player_after_y_movement = player_collider;
		player_after_y_movement.position.y += king.velocity.y * delta_time;

		Rect ground_check_rect = player_collider;
		ground_check_rect.position.y += 0.75;

		if(is_colliding(player_after_x_movement, tile_collider)) {
			king.velocity.x = 0;
		}

		if(is_colliding(player_after_y_movement, tile_collider)) {
			if (king.velocity.y > 0 && !tile.is_crumbling) {
				tile.is_crumbling = true;
				// TODO: Settings for crumble length
				tile.time_till_crumble = 0.5;
				buffer_sound(platform, sounds.tile_crumbles[random_int(sounds.tile_crumbles.size())], 1);
			}
			king.velocity.y = 0;
		}

		if(king.velocity.y >= 0 && is_colliding(ground_check_rect, tile_collider)) {
			king.is_grounded = true;
		}
	}

	king.position.x += king.velocity.x * delta_time;
	king.position.y += king.velocity.y * delta_time;
}

