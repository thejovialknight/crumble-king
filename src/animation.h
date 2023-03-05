// TODO: Should we change sequences to a vector of sequences and have the Sequence struct have either an enum or GASP a string to reference the right one.
// Then, the king can just get the integer index into the vector on load by asking where in the vector is the string or whatnot.
// Might be better and more robust, in all honesty

#pragma once
#include <vector>
#include <string>
#include "vec2.h"
#include "rect.h"
#include "platform.h"
#include "text_parsing.h"

struct Sequence {
	Vec2 origin = Vec2(0, 0);
	std::vector<IRect> frames;
};

struct Sequences {
	Sequence tile_island;
	Sequence tile_center;
	Sequence tile_left;
	Sequence tile_right;
	Sequence bubbling_pot;
	Sequence platter;
	Sequence food_potato;
	Sequence food_watermelon;
	Sequence food_grape;
	Sequence food_chicken;
	Sequence king_idle;
	Sequence king_run;
	Sequence king_jump;
	Sequence king_float;
	Sequence guard_idle;
	Sequence guard_run;
	Sequence guard_jump;
	Sequence guard_end;
	Sequence window;
	Sequence emote_alarm;
	Sequence emote_confused;
};

struct Animator {
	bool is_visible = true;
	Sequence* sequence = nullptr;
	int frame = 0;
	double time_till_next_frame = 0;
	double frame_length = 0.2;
	bool is_flipped = false;
	bool is_looping = true;
	bool is_finished = false; // only true if not looping

	Animator() {}
	Animator(bool is_visible) : is_visible(is_visible) {}
};

void tick_animator(Animator& animator, double delta_time);
PlatformSprite sprite_from_animator(int atlas, Animator& animator, const Vec2& position);
PlatformSprite sprite_from_sequence(int atlas, const Sequence& sequence, int frame, const Vec2& position, bool is_flipped);
Sequences load_sequences(const std::string text);
