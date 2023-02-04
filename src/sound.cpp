#include "sound.h"

Sounds load_sounds(Platform& platform)
{
	Sounds sounds;
	// Menu
	sounds.menu_move = new_sound_handle(platform, "resources/sounds/menu_move.wav");
	sounds.menu_select = new_sound_handle(platform, "resources/sounds/menu_select.wav");
	sounds.menu_reset_data_confirmation = new_sound_handle(platform, "resources/sounds/menu_reset_data_confirmation.wav");
	sounds.menu_reset_data = new_sound_handle(platform, "resources/sounds/menu_reset_data.wav");
	// King
	sounds.king_jump = new_sound_handle(platform, "resources/sounds/king_jump.wav");
	sounds.king_float = new_sound_handle(platform, "resources/sounds/king_float.wav");
	// Food
	sounds.food_prepping = new_sound_handle(platform, "resources/sounds/food_prepping.wav");
	sounds.food_cooked = new_sound_handle(platform, "resources/sounds/food_cooked.wav");
	sounds.food_expired = new_sound_handle(platform, "resources/sounds/food_expired.wav");
	sounds.food_alarm = new_sound_handle(platform, "resources/sounds/food_alarm.wav");
	sounds.food_pickup = new_sound_handle(platform, "resources/sounds/food_pickup.wav");
	// Enemy
	sounds.enemy_seen_king = new_sound_handle(platform, "resources/sounds/enemy_seen_king.wav");
	sounds.enemy_lost_king = new_sound_handle(platform, "resources/sounds/enemy_lost_king.wav");
	// Misc gameplay
	sounds.lost_life = new_sound_handle(platform, "resources/sounds/lost_life.wav");
	sounds.lost_game = new_sound_handle(platform, "resources/sounds/lost_game.wav");
	// Music
	sounds.music_menu = new_sound_handle(platform, "resources/sounds/music_menu.wav");
	sounds.music_victory = new_sound_handle(platform, "resources/sounds/music_victory.wav");
	sounds.music_level1 = new_sound_handle(platform, "resources/sounds/music_level1.wav");
	sounds.music_level2 = new_sound_handle(platform, "resources/sounds/music_level2.wav");
	sounds.music_level3 = new_sound_handle(platform, "resources/sounds/music_level3.wav");
	// Tile crumbles
	sounds.tile_crumbles.push_back(new_sound_handle(platform, "resources/sounds/tile_crumble_1.wav"));
	sounds.tile_crumbles.push_back(new_sound_handle(platform, "resources/sounds/tile_crumble_2.wav"));
	sounds.tile_crumbles.push_back(new_sound_handle(platform, "resources/sounds/tile_crumble_3.wav"));

	return sounds;
}
