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

struct LevelData {
    std::string name;
    char tilemap[ROWS * COLUMNS];
};

enum class LevelState {
    PRE,
    ACTIVE,
    POST,
    HITCH
};

enum class PostLevelBehavior {
    RESTART,
    ADVANCE,
    QUIT
};

struct PostLevelInfo {
    bool ready_to_exit = false; // Needed for Game to advance
    PostLevelBehavior behavior = PostLevelBehavior::RESTART; // True if won level, false if died
};

struct Level { 
    // State
    LevelState state = LevelState::PRE;
    PostLevelInfo post_level_info;
    double time_to_next_state;
    int score = 0;
    bool ready_to_play_dead_sound = false;
    SurfaceMap surface_map;

    // "Entitites"
    King king;
    Food food;
    std::vector<Tile> tiles;
    std::vector<Enemy> enemies;

    // Data
    LevelData* data;
    Sequence* tile_sequence;
    Sequence* window_sequence;

    Level(LevelData* data, Sequences& sequences, Platform& platform);
};

void load_level(Level& level, Sequences& sequences, Sounds& sounds, Platform& platform);
void update_level(Level& level, int atlas, Sequences& sequences, Sounds& sounds, Platform& platform, Settings& settings, double delta_time);
void handle_pre_level(Level& level, int atlas, Sequences& sequences, Sounds& sounds, Platform& platform, Settings& settings, double delta_time);
void handle_active_level(Level& level, int atlas, Sequences& sequences, Sounds& sounds, Platform& platform, Settings& settings, double delta_time);
void handle_post_level(Level& level, int atlas, Sequences& sequences, Platform& platform, Settings& settings, double delta_time);
void handle_hitch_level(Level& level, Settings& settings, double delta_time);
void render_level(Level& level, int atlas, Platform& platform);
void goto_post_level(Level& level, PostLevelBehavior behavior);
int music_from_level_name(std::string& name, Sounds& sounds);
