#include "menu.h"

void tick_menu_list(MenuList& list, Sounds& sounds, Platform& platform)
{
    bool play_move_sound = false;
    // Iterate selection based on input
    if (platform.input.up.just_pressed) { 
        play_move_sound = true;
        list.selection--; 
        if (list.selection < 0) { list.selection = list.buttons.size() - 1; }
    }
    if (platform.input.down.just_pressed) { 
        play_move_sound = true;
        list.selection++; 
        if (list.selection >= list.buttons.size()) { list.selection = 0; }
    }

    if (play_move_sound) {
        buffer_sound(platform, sounds.menu_move, 1);
    }

    // Draw menu items
    for(int i = 0; i < list.buttons.size(); ++i) {
        Vec3 color(0.8, 0.8, 0.8);
        if(i == list.selection) { color = Vec3(0.9, 0.2, 0.2); };
        platform.texts.emplace_back(PlatformText(
            list.buttons[i].text, 
            0,
            32,
            64 + (i * 16),
            color
        ));
    }
}

void tick_main_menu(MainMenu& menu, std::vector<LevelData>& levels, std::vector<int> scores, Sounds& sounds, Platform& platform)
{
    tick_menu_list(menu.list, sounds, platform);

    if (menu.list.selection != 2 && menu.reset_confirmation == true) {
        menu.reset_confirmation = false;
        menu.list.buttons[2].text = "Reset Data";
    }

    if(platform.input.jump.just_pressed) {
        buffer_sound(platform, sounds.menu_select, 1);
    }

    if(menu.state == MainMenuState::MAIN) {
        // Handle selection
        if (platform.input.jump.just_pressed) {
            switch (menu.list.selection) {
            case 0:
                menu.state = MainMenuState::LEVEL_SELECT;
                menu.list.selection = 0;
                populate_level_select_menu(menu.list, levels);
                break;
            case 1:
                menu.state = MainMenuState::HIGH_SCORES;
                menu.list.selection = 0;
                populate_high_scores_menu(menu.list, scores);
                break;
            case 2:
                if (menu.reset_confirmation) {
                    // High Scores text duplicated when we first populate. JIT solution is better!
                    menu.list.buttons[2].text = "Reset Data";
                    menu.should_reset_data = true;
                    menu.reset_confirmation = false;
                    buffer_sound(platform, sounds.menu_reset_data, 1);
                } else {
                    menu.list.buttons[2].text = "Reset all score data?";
                    menu.reset_confirmation = true;
                    buffer_sound(platform, sounds.menu_reset_data_confirmation, 1);
                }
                break;
            case 3:
                platform.window_should_close = true;
                break;
            default:
                break;
            }
        }

        // Quit on escape
        if (platform.input.pause.just_pressed) { 
            platform.window_should_close = true; 
        }
    } else if(menu.state == MainMenuState::LEVEL_SELECT) {
        // Handle selection
        if (platform.input.jump.just_pressed) {
            menu.level_index_to_load = menu.list.selection;
            menu.ready_to_load_level = true;
        }

        // Back to menu on escape
        // TODO: Refactor because of high score one too
        if (platform.input.pause.just_pressed) {
            buffer_sound(platform, sounds.menu_select, 1);
            menu.state = MainMenuState::MAIN;
            menu.list.selection = 0;
            populate_main_menu(menu.list);
        }
    } else if(menu.state == MainMenuState::HIGH_SCORES) {
        // Handle selection
        if(platform.input.jump.just_pressed) {
            if(menu.list.selection == 10) {
                menu.state = MainMenuState::MAIN;
                menu.list.selection = 0;
                populate_main_menu(menu.list);
            }
        }

        // Back to menu on escape
        if (platform.input.pause.just_pressed) {
            buffer_sound(platform, sounds.menu_select, 1); 
            menu.state = MainMenuState::MAIN;
            menu.list.selection = 0;
            populate_main_menu(menu.list);
        }
    }
}

void populate_main_menu(MenuList& list)
{
    list.buttons.clear();
    list.buttons.emplace_back(MenuButton("Level Select"));
    list.buttons.emplace_back(MenuButton("High Scores"));
    list.buttons.emplace_back(MenuButton("Reset Data"));
    list.buttons.emplace_back(MenuButton("Quit"));
}

void populate_level_select_menu(MenuList& list, std::vector<LevelData>& levels)
{
    list.buttons.clear();
    for (LevelData& level : levels) {
        list.buttons.emplace_back(MenuButton(level.name));
    }
}

void populate_high_scores_menu(MenuList& list, std::vector<int>& scores)
{
    list.buttons.clear();
    for(int i = 0; i < 10; ++i) {
        std::string prefix = std::to_string(i + 1) + ". ";
        if(i >= scores.size() || scores[i] == 0) {
            list.buttons.emplace_back(prefix + "-");
        } else {
            list.buttons.emplace_back(MenuButton(prefix + std::to_string(scores[i])));
        }
    }
    list.buttons.emplace_back(MenuButton("<< To Menu"));
}

