#pragma once
#include "vec2.h"
#include "animation.h"

struct Emote {
	Vec2* target_position;
	Vec2 offset_position;
	Animator animator;

	Emote(Vec2* target_position, Vec2 offset_position, Sequence* sequence);
};

void update_emotes(std::vector<Emote>& emotes, double delta_time);
