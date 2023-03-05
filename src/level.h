#pragma once
#include "king.h"
#include "game_state.h"
#include "collision.h"
#include "tiles.h"
#include "food.h"
#include "animation.h"
#include "sound.h"
#include "enemy.h"
#include "hitch.h"
#include "emote.h"

struct LevelData {
    std::string name;
    char tilemap[ROWS * COLUMNS];
};

enum class LevelState {
    PRE,
    ACTIVE,
    WON,
    LOST,
    HITCH,
    QUIT
};

struct Level { 
    // State
    LevelState state = LevelState::PRE;
    double time_to_next_state;
    int score = 0;
    bool ready_to_play_dead_sound = false;
    SurfaceMap surface_map;

    // "Entitites"
    King king;
    Food food;
    std::vector<Tile> tiles;
    std::vector<Enemy> enemies;
    std::vector<Emote> emotes;

    // Data
    LevelData* data;
    Sequence* tile_sequence;
    Sequence* window_sequence;

    Level(LevelData* data, Sequences& sequences, Platform& platform);
};

void load_level(Level& level, Sequences& sequences, Sounds& sounds, Platform& platform);
void tick_level(Level& level, int atlas, Sequences& sequences, Sounds& sounds, Platform& platform, Settings& settings, double delta_time);
void tick_pre_level(Level& level, int atlas, Sequences& sequences, Sounds& sounds, Platform& platform, Settings& settings, double delta_time);
void tick_active_level(Level& level, int atlas, Sequences& sequences, Sounds& sounds, Platform& platform, Settings& settings, double delta_time);
void tick_post_level(Level& level, int atlas, Sequences& sequences, Platform& platform, Settings& settings, double delta_time);
void tick_hitch_level(Level& level, Settings& settings, double delta_time);
void draw_level(Level& level, int atlas, Platform& platform);
int music_from_level_name(std::string& name, Sounds& sounds);
