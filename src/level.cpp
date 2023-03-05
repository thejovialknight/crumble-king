#include <iostream>
#include <SDL.h>
#include "level.h"

Level::Level(LevelData* data, Sequences& sequences, Platform& platform) : data(data)
{
    // Initialize state
    food.sequences.push_back(&sequences.food_chicken);
    food.sequences.push_back(&sequences.food_grape);
    food.sequences.push_back(&sequences.food_watermelon);
    food.sequences.push_back(&sequences.food_potato);
    food.platter_sequence = &sequences.platter;
    food.bubbling_pot_sequence = &sequences.bubbling_pot;
    food.animator.sequence = food.sequences[random_int(food.sequences.size())];
    tile_sequence = &sequences.tile_center;
    window_sequence = &sequences.window;
    king.animator.sequence = &sequences.king_idle;
}

void load_level(Level& level, Sequences& sequences, Sounds& sounds, Platform& platform)
{
    level.tiles.clear();
    level.food.windows.clear();
    level.enemies.clear();
    // Populate tiles from tilemap
    int i = 0;
    for (const char c : level.data->tilemap) {
        if (c == '\n') { continue; }
        if (c == 'K') { level.king.position = grid_position_from_index(i); }
        if (c == 'F') { 
            Vec2 p = grid_position_from_index(i);
            p.y -= 8;
            level.food.windows.emplace_back(Window(p));
        }
        if (c == 'E') { 
            Vec2 p = grid_position_from_index(i);
            p.y += 8;
            level.enemies.emplace_back(Enemy(p)); 
        }
        if (c == '#') { level.tiles.emplace_back(grid_position_from_index(i)); }
        i++;
    }
    
    level.surface_map = get_surface_map(level.tiles);
    int k = 2;

    // Initialize data
    level.score = 0;
    level.food.state = FoodState::INACTIVE;
    level.food.time_to_next_phase = 4; // TODO: settings
    level.food.level_complete = false;
    level.food.animator.sequence = level.food.sequences[random_int(level.food.sequences.size())];
    level.king.velocity = Vec2(0, 0);
    level.king.jump_state = JumpState::GROUND;
    level.king.acceleration_mod = 1;
    level.king.animator.sequence = &sequences.king_idle;
    for(Enemy& enemy : level.enemies) {
        enemy.state = EnemyState::PATROL;
        enemy.velocity = Vec2(0, 0);
        if(random_int(2) == 0) {
            enemy.stored_x_direction = 1;
        } else {
            enemy.stored_x_direction = -1;
        }
    }
    // Initiate pre level
    level.state = LevelState::PRE;
    level.time_to_next_state = 2;
    stop_music();
}

void tick_level(Level& level, int atlas, Sequences& sequences, Sounds& sounds, Platform& platform, Settings& settings, double delta_time)
{
    switch(level.state) {
    case LevelState::PRE:
        tick_pre_level(level, atlas, sequences, sounds, platform, settings, delta_time);
        break;
    case LevelState::ACTIVE:
        tick_active_level(level, atlas, sequences, sounds, platform, settings, delta_time);
        break;
    case LevelState::WON:
        level.king.position.y -= 300 * delta_time; // TODO: SETTINGS!
        tick_post_level(level, atlas, sequences, platform, settings, delta_time);
        break;
    case LevelState::LOST:
        tick_post_level(level, atlas, sequences, platform, settings, delta_time);
        break;
    case LevelState::HITCH:
        tick_hitch_level(level, settings, delta_time);
        break;
    }
    
    // Menu on escape
    if (platform.input.pause.just_pressed) {
        buffer_sound(platform, sounds.menu_select, 1);
        level.state = LevelState::QUIT;
    }

    draw_level(level, atlas, platform);
}

void tick_active_level(Level& level, int atlas, Sequences& sequences, Sounds& sounds, Platform& platform, Settings& settings, double delta_time)
{
    platform.background_color = Vec3(0, 0, 0);
    level.surface_map = get_surface_map(level.tiles);
    HitchInfo hitch(false, 0);

    tick_king(level.king, platform, sequences, sounds, settings, delta_time);
    tick_food(level.score, level.food, level.king, sounds, platform, settings, hitch, delta_time);
    tick_enemies(level.enemies, level.king, level.tiles, level.surface_map, level.emotes, sequences, sounds, platform, delta_time);
    tick_tiles(level.tiles, delta_time);
    resolve_king_velocity(level.king, level.tiles, sounds, platform, delta_time);

    if(hitch.should_hitch == true) {
        level.state = LevelState::HITCH;
        level.time_to_next_state = hitch.length;
    }

    if(is_king_dead(level.king, platform) || is_king_caught(level.enemies, level.king)) {
        stop_music();
        level.ready_to_play_dead_sound = true;
        level.time_to_next_state = 4; // TODO: Settings!
        level.state = LevelState::LOST;
    } else if(level.food.level_complete) {
        set_music(platform, sounds.music_victory, 1);
        level.time_to_next_state = 4; // TODO: Settings!
        level.state = LevelState::WON;
    }
}

void tick_pre_level(Level& level, int atlas, Sequences& sequences, Sounds& sounds, Platform& platform, Settings& settings, double delta_time)
{
    level.time_to_next_state -= delta_time;
    if(level.time_to_next_state <= 0 || platform.input.jump.just_pressed) {
        set_music(platform, music_from_level_name(level.data->name, sounds), 1);
        level.state = LevelState::ACTIVE;
    }

    for(Enemy& enemy : level.enemies) {
        enemy.animator.sequence = &sequences.guard_idle;
    }
}

void tick_post_level(Level& level, int atlas, Sequences& sequences, Platform& platform, Settings& settings, double delta_time)
{
    for (Enemy& enemy : level.enemies) {
        enemy.animator.sequence = &sequences.guard_end;
        enemy.animator.frame_length = 0.2;
        tick_animator(enemy.animator, delta_time);
    }

    level.time_to_next_state -= delta_time;
    if (platform.input.jump.just_pressed) {
        level.time_to_next_state = -1;
    }
}

void tick_hitch_level(Level& level, Settings& settings, double delta_time) {
    if(level.food.state == FoodState::RESET) tick_food_reset(level.food, settings, delta_time);
    
    level.time_to_next_state -= delta_time;
    if(level.time_to_next_state <= 0) level.state = LevelState::ACTIVE;
}

void draw_level(Level& level, int atlas, Platform& platform)
{
    // Draw windows
    for(const Window& window : level.food.windows) {
        int frame = 0;
        if(!window.is_active) { frame = 1; }
        put_sprite(platform, sprite_from_sequence(
            atlas,
            *level.window_sequence,
            frame,
            window.spawn_position,
            false
        ));
    }
    // Draw food if active
    if (level.food.state != FoodState::INACTIVE) {
        put_sprite(platform, sprite_from_animator(
            atlas, 
            level.food.animator,
            level.food.position
        ));
    }
    // Draw tiles
    for (const Tile& tile : level.tiles) {
        int visible_health = tile.health;

        if (tile.is_crumbling) { visible_health--; }
        if (visible_health <= 0) { continue; }

        put_sprite(platform, sprite_from_sequence(
            atlas,
            *level.tile_sequence,
            visible_health,
            tile.position,
            false // is_flipped could be randomized? Hmm, probably not.
        ));
    }
    // Draw enemies
    for(Enemy& enemy : level.enemies) {
        put_sprite(platform, sprite_from_animator(
            atlas,
            enemy.animator,
            enemy.position
        ));
        if(enemy.emote.animator.is_visible) {
            put_sprite(platform, sprite_from_animator(
                atlas,
                enemy.emote.animator,
                enemy.position + Vec2(-8, -16) // TODO: SETTINGS!
            ));
        }
    }
    // Draw king
    put_sprite(platform, sprite_from_animator(
        atlas,
        level.king.animator,
        level.king.position
    ));
    // Draw points
    platform.texts.emplace_back(PlatformText(
        "Points: " + std::to_string(level.score),
        64,
        100,
        100,
        Vec3(0.9, 0.2, 0.2)
    ));

    SDL_RenderPresent(platform.renderer);
}

int music_from_level_name(std::string& name, Sounds& sounds)
{
    if(name == "Archipelago") {
        return sounds.music_level1;
    } else if(name == "Out of the Frying Pan...") {
        return sounds.music_level2;
    } else if(name == "Xavier's Cave") {
        return sounds.music_level3;
    }
    return sounds.music_menu;
}
