#pragma once
#include <vector>
#include "level.h"

struct TowerData {
	std::string name;
	std::vector<LevelData*> levels;
};

struct Tower {
	Level level;
	TowerData* data;
	int level_index = 0;
	int lives_remaining = 3; // TODO: Settings
	bool ready_to_exit = false;
	int total_score = 0;

	Tower(TowerData* data, Level level) : data(data), level(level) {}
};

void update_tower(Tower& tower, int atlas, Sequences& sequences, Sounds& sounds, Settings& settings, Platform& platform, double delta_time);
