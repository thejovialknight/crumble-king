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
