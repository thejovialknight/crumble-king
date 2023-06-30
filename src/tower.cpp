#include "tower.h"

void tick_tower(Tower& tower, int atlas, Sequences& sequences, Sounds& sounds, Settings& settings, Platform& platform, double delta_time)
{
    tick_level(tower.level, atlas, sequences, sounds, platform, settings, delta_time);

    platform.texts.emplace_back(PlatformText(
        "Lives: " + std::to_string(tower.lives_remaining),
        0,
        128,
        32,
        Vec3(0.9, 0.2, 0.2)
    ));

    if(tower.level.ready_to_play_dead_sound) {
        tower.level.ready_to_play_dead_sound = false;
        if(tower.lives_remaining <= 0) {
            buffer_sound(platform, sounds.lost_game, 1);
        } else {
            buffer_sound(platform, sounds.lost_life, 1);
        }
    }

    if (tower.level.state == LevelState::QUIT) {
        tower.state = TowerState::QUIT;
    } else if(tower.level.state == LevelState::WON && tower.level.time_to_next_state <= 0) {
        tower.total_score += tower.level.score;
        if(tower.level_index >= tower.data->levels.size() - 1) {
            tower.state = TowerState::GAME_OVER;
        } else {
            tower.level_index++;
            tower.level.data = tower.data->levels[tower.level_index];
            load_level(tower.level, sequences, sounds, platform);
        }
    } else if(tower.level.state == LevelState::LOST && tower.level.time_to_next_state <= 0) {
        tower.total_score += tower.level.score;
        tower.lives_remaining -= 1;
        if(tower.lives_remaining < 0) {
            tower.state = TowerState::GAME_OVER;
        } else {
            load_level(tower.level, sequences, sounds, platform);
        }
    }
}
