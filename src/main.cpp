#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "platform.h"
#include "game.h"
#include "random.h"

// Entry point of app
int main(int argc, char* argv[])
{
    Platform platform;
    init_platform(platform);

    Game game;
    init_game(game, platform);
    game.state = GameState::MENU;
    game.menu = new MainMenu();
    populate_main_menu(game.menu->list);

    set_random_seed();

    while (!platform.window_should_close) {
        double delta_time = get_delta_time(platform);
        update_game(game, platform, delta_time);
        update_platform(platform);
    }

    deinit_platform();
    return 0;
}
