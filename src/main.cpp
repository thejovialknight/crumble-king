#include "platform.h"
#include "game.h"
#include "random.h"

// Entry point of app
int main(void)
{
    // ==============
    // Initialization
    // ==============
    // Initialize platform
    Platform platform; // Current state of the platform layer
    init_platform(platform); // Call raylib initialization stuff

    // Initialize game
    Game game;
    init_game(game, platform);
    game.state = GameState::MENU;
    game.menu = new MainMenu();
    populate_main_menu(game.menu->list);

    // Seed randomness
    set_random_seed();

    // ==============
    // Main game loop
    // ==============
    while (!platform.window_should_close)
    {
        update_game(game, platform, 0.016); // Update game state
        update_platform(platform); // Apply platform to system
    }

    // =======
    // Cleanup
    // =======
    deinit_platform(); // Clean up raylib stuff
    return 0;
}
