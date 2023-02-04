#pragma once
#include "platform.h"

struct Sounds {
	// Menu
	int menu_move;
	int menu_select;
	int menu_reset_data;
	int menu_reset_data_confirmation;
	// King
	int king_jump;
	int king_float;
	// Food
	int food_prepping;
	int food_cooked;
	int food_expired;
	int food_alarm;
	int food_pickup;
	// Enemy
	int enemy_seen_king;
	int enemy_lost_king;
	// Misc gameplay
	int lost_life;
	int lost_game;
	// Music
	int music_menu;
	int music_victory;
	int music_level1;
	int music_level2;
	int music_level3;
	// Tile crumble
	std::vector<int> tile_crumbles;
};

Sounds load_sounds(Platform& platform);
