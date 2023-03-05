#pragma once
#include <vector>
#include "random.h"
#include "king.h"
#include "sound.h"
#include "hitch.h"

struct Window {
	Vec2 spawn_position;
	bool is_active = true;

	Window(Vec2 spawn_position) : spawn_position(spawn_position) {}
};

enum class FoodState {
	INACTIVE,
	COOKING,
	COOKED,
	POT,
	RESET
};

struct Food {
	Vec2 position = Vec2(0,0);
	FoodState state = FoodState::INACTIVE;
	double time_to_next_phase = 0;
	double time_to_alarm = 0;
	Animator animator;
	std::vector<Window> windows;
	std::vector<Sequence*> sequences;
	Sequence* platter_sequence;
	Sequence* bubbling_pot_sequence;
	int current_window;
	bool level_complete = false;
};

void tick_food(int& out_score, Food& food, King& king, Sounds& sounds, Platform& platform, Settings& settings, HitchInfo& out_hitch, double delta_time);
void tick_food_reset(Food& food, Settings& settings, double delta_time);
void hide_food(Food& food, double inactive_length);
