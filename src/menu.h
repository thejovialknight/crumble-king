#pragma once
#include "platform.h"
#include "game_state.h"
#include "level.h"
#include "sound.h"

struct MenuButton {
    std::string text;

    MenuButton(std::string text) : text(text) {}
};

struct MenuList {
    int selection = 0;
    std::vector<MenuButton> buttons;
};

enum class MainMenuState {
    MAIN,
    LEVEL_SELECT,
    HIGH_SCORES
};

struct MainMenu {
    MainMenuState state = MainMenuState::MAIN;
    MenuList list;
    int level_index_to_load = 0;
    bool ready_to_load_level = false;
    bool should_reset_data = false;
    bool reset_confirmation = false;
};

void handle_menu_list(MenuList& list, Sounds& sounds, Platform& platform);
void update_main_menu(MainMenu& menu, std::vector<LevelData>& levels, std::vector<int> scores, Sounds& sounds, Platform& platform);
void populate_main_menu(MenuList& list);
void populate_high_scores_menu(MenuList& list, std::vector<int>& scores);
void populate_level_select_menu(MenuList& list, std::vector<LevelData>& levels);
