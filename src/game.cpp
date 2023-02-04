#include "game.h"

void init_game(Game& game, Platform& platform)
{
    game.sounds = load_sounds(platform);
    buffer_sound(platform, game.sounds.music_menu, 1);

    // TODO: Functional = 
    populate_settings(get_file_text("resources/config/config.txt"), game.settings);
    populate_sequences(get_file_text("resources/sprites/sequences.txt"), game.sequences);
    
    // High scores. TODO: put in high_scores file
    std::string score_text = get_file_text("resources/save/scores.txt");
    for(int i = 0; i < score_text.length(); ) {
        game.high_scores.push_back(pull_int_before_char('\n', score_text, i));
    }

    game.atlas = new_texture_handle(platform, "resources/sprites/sprite_atlas.png");
    std::string level_str = get_file_text("resources/levels/levels.txt");
    LevelData level_data;
    int start = 0;
    int length = 0; // must equal start at beginning
    for(int i = 0; i < level_str.length(); ++i) {
        if(level_str[i] == '@') {
            i++;
            // If this is the first level in string
            if(length != start) { game.levels.push_back(level_data); }

            // Get title
            const int max_title_length = 50;
            int current_title_length = 0;
            level_data.name = "";
            while(current_title_length < max_title_length && level_str[i] != ':') {
                level_data.name += level_str[i];
                i++;
            }
            start = i + 1;
            length = 0;
        } else {
            if (level_str[i] == '\n') { continue; }

            level_data.tilemap[length] = level_str[i];
            length++;
        }
    }
    game.levels.push_back(level_data);

    // Hardcoded towers. TODO: Make pipeline soon.
    TowerData tower;
    tower.name = "Tower 1";
    tower.levels.push_back(&game.levels[0]);
    tower.levels.push_back(&game.levels[1]);
    tower.levels.push_back(&game.levels[2]);
    game.towers.push_back(tower);
}

void update_game(Game& game, Platform& platform, double delta_time)
{
    // TODO: On function key or something. This is a bit slow every frame, obviously.
    populate_settings(get_file_text("resources/config/config.txt"), game.settings);

    switch(game.state) {
    case GameState::MENU :
        update_main_menu(*game.menu, game.levels, game.high_scores, game.sounds, platform);
        if (game.menu->should_reset_data) {
            reset_data(game, platform);
            game.menu->should_reset_data = false;
        }

        if(game.menu->ready_to_load_level && game.menu->level_index_to_load < game.levels.size()) {
            int level_index_to_load = game.menu->level_index_to_load;
            delete game.menu;
            game.state = GameState::TOWER;
            // Hardcoded tower at index 0. TODO: Implement from tower pipeline
            game.tower = new Tower(&game.towers[0], Level(&game.levels[level_index_to_load], game.sequences, platform));
            game.tower->level_index = level_index_to_load; // CAREFUL! IN THE FUTURE THIS INDEX WON'T HAVE ANY CORRESPONDENCE TO THE TOWER INDEX
            game.tower->lives_remaining = 3;
            load_level(game.tower->level, game.sequences, game.sounds, platform);
            stop_sound(platform, game.sounds.music_menu);
        }
        platform.background_color = Vec3(0, 0, 0);
        break;
    case GameState::TOWER :
        update_tower(*game.tower, game.atlas, game.sequences, game.sounds, game.settings, platform, delta_time);
        if(game.tower->ready_to_exit) {
            // TODO: Should we ALWAYS push back score? What about when we just quit?
            add_high_score(game.high_scores, game.tower->total_score);
            write_high_scores(game.high_scores, platform);
            buffer_sound(platform, game.sounds.music_menu, 1);
            return_to_menu(game);
        }
        break;
    default :
        break;
    }
}

void return_to_menu(Game& game)
{
    delete game.tower;
    game.state = GameState::MENU;
    game.menu = new MainMenu();
    game.menu->state = MainMenuState::LEVEL_SELECT;
    populate_level_select_menu(game.menu->list, game.levels);
}

void reset_data(Game& game, Platform& platform)
{
    game.high_scores.clear();
    write_high_scores(game.high_scores, platform);
    //SaveFileText("resources/save/scores.txt", "0\n0\n0\n0\n0\n0\n0\n0\n0\n0\n");
    std::string data = "-1\n";
}
