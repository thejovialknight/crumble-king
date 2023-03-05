#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "platform.h"
#include "game.h"

int main(int argc, char* argv[])
{
    Platform platform;
    init_platform(platform);

    Game game;
    init_game(game, platform);

    while (!platform.window_should_close) {
        double delta_time = get_delta_time(platform);
        tick_game(game, platform, delta_time);
        update_platform(platform);
    }

    return 0;
}
