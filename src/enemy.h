#pragma once
#include <vector>
#include "rect.h"
#include "king.h"
#include "tiles.h"
#include "collision.h"
#include "emote.h"

enum class EnemyState {
	PATROL,
	CHASE,
	PAUSE,
	LOST
};

struct Enemy {
	EnemyState state = EnemyState::PATROL;
	Vec2 position;
	Vec2 velocity;
	double stored_x_direction; // -1 for left, 1 for right
	double time_to_unpause = 0;
	Animator animator;
    Rect collider = Rect(-4, -16, 12, 16);

	Enemy(Vec2 position) : position(position) {}
};

void update_enemies(std::vector<Enemy>& enemies, King& king, std::vector<Tile>& tiles, SurfaceMap& surface_map, std::vector<Emote>& emotes, Sequences& sequences, double delta_time);
bool is_king_caught(std::vector<Enemy>& enemies, King& king);
// Returns -1 if not on platform. TODO: Maybe PlatformHandle with bool?
Surface* surface_from_entity_position(Vec2& position, std::vector<Tile>& tiles, SurfaceMap& surface_map);
