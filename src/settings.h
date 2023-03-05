#pragma once
#include <string>

struct Settings {
	double king_max_speed;
	double king_acceleration;
	double king_jump_speed;
	double jump_buffer_length;
	double coyote_time_length;
	double float_velocity;
	double float_start_gravity_scale;
	double float_target_gravity_scale;
	double float_gravity_lerp_speed;
	double fall_gravity_scale;
	double food_inactive_length;
	double food_cook_length;
	double food_expiration_length;
	double food_distance_to_eat;
	double gravity;
};

struct SettingsField {
	std::string key = "";
	std::string value = "";
};

Settings load_settings(const std::string& text);
bool try_field(const SettingsField& field, const std::string expected_key, std::string& out_value);
bool try_int_field(const SettingsField& field, const std::string expected_key, int& out_value);
bool try_double_field(const SettingsField& field, const std::string expected_key, double& out_value);
