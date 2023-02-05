#include "collision.h"

bool is_colliding(Rect& a, Rect& b)
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

bool is_colliding(Rect& a_col, Vec2& a_pos, Rect& b_col, Vec2& b_pos)
{
	Rect off_a_col = offset_collider(a_col, a_pos);
	Rect off_b_col = offset_collider(b_col, b_pos);
	return is_colliding(off_a_col, off_b_col);
}

Rect offset_collider(Rect& collider, Vec2& position)
{
	return Rect(
		position.x + collider.position.x,
		position.y + collider.position.y,
		collider.size.x,
		collider.size.y
	);
}

void resolve_king_velocity(King& king, std::vector<Tile>& tiles, Sounds& sounds, Platform& platform, double delta_time)
{
	Rect king_col = offset_collider(king.collider, king.position);
	king.is_grounded = false;

	// Loop through collision checks
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
				// TODO: Settings for crumble length
				tile.time_till_crumble = 0.5;
				buffer_sound(platform, sounds.tile_crumbles[random_int(sounds.tile_crumbles.size())], 1);
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
}

