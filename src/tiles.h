#pragma once
#include <vector>
#include <unordered_map>
#include "vec2.h"
#include "utils.h"

#define COLUMNS 40
#define ROWS 23

struct Tile {
	int health = 3;
	Vec2 position;
	bool is_crumbling = false;
	double time_till_crumble = 0;

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
void put_tile(bool tiles[], int x, int y);
void update_tiles(std::vector<Tile>& tiles, double delta_time);
SurfaceMap get_surface_map(std::vector<Tile>& tiles);
