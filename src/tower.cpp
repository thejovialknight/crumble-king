#include "tower.h"

void tick_tower(Tower& tower, int atlas, Sequences& sequences, Sounds& sounds, Settings& settings, Platform& platform, double delta_time)
{
    tick_level(tower.level, atlas, sequences, sounds, platform, settings, delta_time);
    platform.texts.emplace_back(PlatformText(
        "Lives: " + std::to_string(tower.lives_remaining),
        64,
        600,
        100,
        Vec3(0.9, 0.2, 0.2)
    ));

    // Handle death sounds. TODO: This sort of two way communication might be best
    // done with a function call to try to reduce the fuckiness of this calling context.
    if(tower.level.ready_to_play_dead_sound) {
        tower.level.ready_to_play_dead_sound = false;
        if(tower.lives_remaining <= 0) {
            buffer_sound(platform, sounds.lost_game, 1);
        } else {
            buffer_sound(platform, sounds.lost_life, 1);
        }
    }

    bool exit_to_high_scores = false;
    if (tower.level.post_level_info.behavior == PostLevelBehavior::QUIT) {
        tower.ready_to_exit = true;
    } else if(tower.level.post_level_info.ready_to_exit) {
        tower.total_score += tower.level.score;
        if(tower.level.post_level_info.behavior == PostLevelBehavior::ADVANCE) { // if won
            if(tower.level_index >= tower.data->levels.size() - 1) {
                tower.ready_to_exit = true;
            } else {
                tower.level_index++;
                tower.level.data = tower.data->levels[tower.level_index];
                load_level(tower.level, sequences, sounds, platform);
            }
        } else { // if died
            platform.background_color = Vec3(0, 0, 0);
            tower.lives_remaining -= 1;
            if(tower.lives_remaining < 0) {
                tower.ready_to_exit = true;
            } else {
                load_level(tower.level, sequences, sounds, platform);
            }
        }
    }
}

