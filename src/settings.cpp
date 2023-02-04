#include "settings.h"

// Takes a string and 
void populate_settings(const std::string& text, Settings& settings)
{
	// Loop through entire settings string, populating fields as we go
	for(int i = 0; i < text.length(); ++i) {
		// Skip to next key
		while(text[i] != '#' && i < text.length()) {
			i++;
		}
		i++; // Skip '#' char

		SettingsField field;

		// Get key
		while(text[i] != '=') {
			field.key += text[i];
			i++;
		}
		i++; // Skip '=' char

		// Get value
		while(text[i] != '\n' && text[i] != '\0') {
			field.value += text[i];
			i++;
		}
			
		// Try to populate fields
		// KING
		if(try_double_field(field, "KING_MAX_SPEED", settings.king_max_speed)) continue;
		if(try_double_field(field, "KING_ACCELERATION", settings.king_acceleration)) continue;
		if(try_double_field(field, "KING_JUMP_SPEED", settings.king_jump_speed)) continue;
		if(try_double_field(field, "KING_JUMP_BUFFER_LENGTH", settings.jump_buffer_length)) continue;
		if(try_double_field(field, "KING_COYOTE_TIME", settings.coyote_time_length)) continue;
		if(try_double_field(field, "KING_FLOAT_VELOCITY", settings.float_velocity)) continue;
		if(try_double_field(field, "KING_FLOAT_START_GRAVITY_SCALE", settings.float_start_gravity_scale)) continue;
		if(try_double_field(field, "KING_FLOAT_TARGET_GRAVITY_SCALE", settings.float_target_gravity_scale)) continue;
		if(try_double_field(field, "KING_FLOAT_GRAVITY_LERP_SPEED", settings.float_gravity_lerp_speed)) continue;
		if(try_double_field(field, "KING_FALL_GRAVITY_SCALE", settings.fall_gravity_scale)) continue;
		// FOOD
		if(try_double_field(field, "FOOD_INACTIVE_LENGTH", settings.food_inactive_length)) continue;
		if(try_double_field(field, "FOOD_COOK_LENGTH", settings.food_cook_length)) continue;
		if(try_double_field(field, "FOOD_EXPIRATION_LENGTH", settings.food_expiration_length)) continue;
		if(try_double_field(field, "FOOD_DISTANCE_TO_EAT", settings.food_distance_to_eat)) continue;
		// MISC
		if(try_double_field(field, "GRAVITY", settings.gravity)) continue;
	}
}

bool try_field(const SettingsField& field, const std::string expected_key, std::string& out_value)
{
	if (field.key == expected_key) {
		out_value = field.value;
		return true;
	}
	return false;
}

bool try_int_field(const SettingsField& field, const std::string expected_key, int& out_value)
{
	std::string str_out;
	if (try_field(field, expected_key, str_out)) {
		out_value = stoi(str_out);
		return true;
	}
	return false;
}

bool try_double_field(const SettingsField& field, const std::string expected_key, double& out_value)
{
	std::string str_out;
	if (try_field(field, expected_key, str_out)) {
		out_value = stod(str_out);
		return true;
	}
	return false;
}

