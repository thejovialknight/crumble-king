#pragma once
#include "vec2.h"
#include "animation.h"

struct Emote {
	Animator animator = Animator(false);
	double time_to_disappear;
};

void tick_emote(Emote& emote, double delta_time);
void activate_emote(Emote& emote, Sequence* sequence, double length);
