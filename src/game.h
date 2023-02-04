#pragma once
#include <vector>
#include <string>
#include "level.h"
#include "menu.h"
#include "game.h"
#include "game_state.h"
#include "settings.h"
#include "animation.h"
#include "sound.h"
#include "tower.h"
#include "text_parsing.h"
#include "high_scores.h"

struct Game {
    // Scenes
    GameState state;
    Tower* tower;
    MainMenu* menu;

    // Data
    Settings settings;
    Sequences sequences;
    Sounds sounds;
    int atlas; // Texture index
	std::vector<LevelData> levels;
    std::vector<TowerData> towers;
    std::vector<int> high_scores;
};

void init_game(Game& game, Platform& platform);
void update_game(Game& game, Platform& platform, double delta_time);
void return_to_menu(Game& game);
void reset_data(Game& game, Platform& platform);
