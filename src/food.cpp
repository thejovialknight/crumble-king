// CURRENT PROGRESS: Happens after the call to update_food.
// Put a breakpoint in level on hitch and windows are set correctly even in there.
// It probably happens in the next tick somehow.

#include "food.h"

void update_food(int& out_score, Food& food, King& king, Sounds& sounds, Platform& platform, Settings& settings, HitchInfo& out_hitch, double delta_time)
{
	const double inactive_length = settings.food_inactive_length;
	const double cook_length = settings.food_cook_length;
	const double expiration_length = settings.food_expiration_length;
	const double alarm_pulse_length = 0.25; // TODO: Settings!!
	const double distance_to_eat = 16; // TODO: Settings!
	const double pot_expiration_length = 10; // TODO: ^^!
	const double reset_length = 2;
	const int high_points = 100; // ^^
	const int low_points = 50; // ^
	const int end_points = 200; //
	const Vec2 spawn_offset(0, 16);
	
	// Handle countdown
	food.time_to_next_phase -= delta_time;
	if(food.state == FoodState::INACTIVE) {
		if (food.time_to_next_phase <= 0) {
			buffer_sound(platform, sounds.food_prepping, 1);

			// Check if time for pot
			int pot_window_index = -1;
			int window_count = 0;
			for(int i = 0; i < food.windows.size(); ++i) {
				if(food.windows[i].is_active) {
 					window_count++;
					pot_window_index = i; // TODO: Do we need pot_window_index? Can we do with just current_window?
					food.current_window = i;
				}
			}

			if(window_count <= 1) {
				food.state = FoodState::POT;
				food.time_to_next_phase = pot_expiration_length;
				food.animator.sequence = food.bubbling_pot_sequence;
				food.position = food.windows[pot_window_index].spawn_position + spawn_offset;
			} else {
				food.state = FoodState::COOKING;
				food.time_to_next_phase = cook_length;
				food.animator.sequence = food.platter_sequence;
				// Find eligible window
				bool found_eligible_window = false;
				while(!found_eligible_window) {
					food.current_window = random_int(food.windows.size());
					if(!food.windows[food.current_window].is_active) { continue; }
					found_eligible_window = true;
					// TODO: I suppose food position is technically redundant
					food.position = food.windows[food.current_window].spawn_position + spawn_offset;
				}
			}
		}
	} else if(food.state == FoodState::COOKING) {
		if(food.time_to_next_phase <= 0) {
			food.state = FoodState::COOKED;
			food.time_to_next_phase = expiration_length;
			food.time_to_alarm = expiration_length / 2;
			buffer_sound(platform, sounds.food_cooked, 1);
			food.animator.sequence = food.sequences[random_int(food.sequences.size())];
		}
	} else if(food.state == FoodState::COOKED) {
		// Handle alarm
		food.time_to_alarm -= delta_time;
		if(food.time_to_alarm <= 0) {
			food.time_to_alarm = alarm_pulse_length;
			food.animator.is_visible = !food.animator.is_visible;
			buffer_sound(platform, sounds.food_alarm, 1);
		}

		// Handle inactivation
		if(food.time_to_next_phase <= 0) { 
			buffer_sound(platform, sounds.food_expired, 1);
			hide_food(food, inactive_length);
		} else if(abs((king.position - food.position).magnitude()) <= distance_to_eat) {
			if(food.time_to_next_phase > expiration_length / 2) {
				out_score += high_points;
			} else {
				out_score += low_points;
			}

			out_hitch = HitchInfo(true, 0.33); // SETTINGS!!
			buffer_sound(platform, sounds.food_pickup, 1);
			hide_food(food, inactive_length);
			king.acceleration_mod *= 0.75;
			food.windows[food.current_window].is_active = false;

			bool some_window_is_active = false;
			for(Window& window : food.windows) {
				if(window.is_active) { 
 					some_window_is_active = true; 
					break;
				}
			}
			if(!some_window_is_active) {
				food.state = FoodState::RESET;
				food.time_to_alarm = 0;
				food.time_to_next_phase = reset_length;
				out_hitch = HitchInfo(true, reset_length);
			}
		}
	} else if (food.state == FoodState::POT) {
		if(food.time_to_next_phase <= 0) {
			// TODO: Same functionality as switching to cooked in cooking
			food.state = FoodState::COOKED;
			food.time_to_next_phase = expiration_length;
			food.time_to_alarm = expiration_length / 2;
			buffer_sound(platform, sounds.food_cooked, 1);
			food.animator.sequence = food.sequences[random_int(food.sequences.size())];
		} else {
			bool king_falling = king.velocity.y > 0;
			bool king_x_target = king.position.x < food.position.x + 16
				&& king.position.x > food.position.x - 16;
			bool king_y_target = king.position.y < food.position.y + 8
				&& king.position.y > food.position.y - 8;

			if(king_falling && king_x_target && king_y_target) {
				out_score += end_points;
				buffer_sound(platform, sounds.king_float, 1);
				food.level_complete = true;
			}
		}
	}

	iterate_animator(food.animator, delta_time);
};

void update_food_reset(Food& food, Settings& settings, double delta_time) 
{
	const double alarm_pulse_length = 0.25; // TODO: Settings!! NOW DUPLICATED WARNING!!!!!

	food.time_to_alarm -= delta_time;
	food.time_to_next_phase -= delta_time;
	if (food.time_to_next_phase <= 0) {
		for (Window& window : food.windows) {
			window.is_active = true;
		}
		hide_food(food, settings.food_inactive_length);
	}
	else if (food.time_to_alarm <= 0) {
		food.time_to_alarm = alarm_pulse_length; // SETTINGS!!
		for (Window& window : food.windows) {
			window.is_active = !window.is_active;
		}
	}
}

void hide_food(Food& food, double inactive_length)
{
	// Set visible because it is intermittently set invisible in COOKED state
	food.animator.is_visible = true; 
	food.state = FoodState::INACTIVE;
	food.time_to_next_phase = inactive_length;
}
