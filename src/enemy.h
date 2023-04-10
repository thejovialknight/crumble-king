#pragma once
#include <vector>
#include "rect.h"
#include "king.h"
#include "tiles.h"
#include "collision.h"
#include "emote.h"
#include "hitch.h"
#include "sound.h"

enum class EnemyState {
	PATROL,
	CHASE,
	PAUSE,
	LOST,
	SEENFREEZE
};

struct Enemy {
	EnemyState state = EnemyState::PATROL;
	Vec2 position;
	Vec2 velocity;
	double stored_x_direction = 1; // -1 for left, 1 for right
	double time_to_unpause = 0;
	double visual_y_offset = 0;
	double visual_y_velocity = 0;
	Animator animator;
    Rect collider = Rect(-4, -12, 9, 12);
	Emote emote;

	Enemy(Vec2 position) : position(position) {}
};

void tick_enemies(std::vector<Enemy>& enemies, King& king, std::vector<Tile>& tiles, SurfaceMap& surface_map, std::vector<Emote>& emotes, Sequences& sequences, Sounds& sounds, Settings& settings, Platform& platform, double delta_time);
bool is_king_caught(std::vector<Enemy>& enemies, King& king);
// Returns -1 if not on platform. TODO: Maybe PlatformHandle with bool?
Surface* surface_from_entity_position(Vec2& position, std::vector<Tile>& tiles, SurfaceMap& surface_map);
double get_stored_x_direction(Enemy& enemy);
