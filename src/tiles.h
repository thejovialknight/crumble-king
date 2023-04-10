#pragma once
#include <vector>
#include <unordered_map>
#include "vec2.h"
#include "utils.h"
#include "rect.h"

#define COLUMNS 40
#define ROWS 23

enum TileOrientation {
	LEFT,
	CENTER,
	RIGHT,
	ISLAND
};

struct Tile {
	int health = 3;
	Vec2 position;
	bool is_crumbling = false;
	int visible_health = 3;
	double time_till_crumble = 0;
	Rect collider = Rect(-8, -8, 16, 16);
	TileOrientation orientation = TileOrientation::RIGHT;

	Tile(Vec2 position) : position(position) {}

};

struct Surface {
	int left_edge_tile;
	int right_edge_tile;
};

struct SurfaceMap {
	std::vector<Surface> surfaces;
	std::unordered_map<int, int> tile_surface_indices;
	//std::unordered_map<IVec2, Surface*, IVec2Hasher> tile_surface_map; 
};

Vec2 grid_position_from_index(int i);
void tick_tiles(std::vector<Tile>& tiles, double delta_time);
void update_tile_orientation(std::vector<Tile>& tiles);
SurfaceMap get_surface_map(std::vector<Tile>& tiles);
