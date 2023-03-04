#include "emote.h"

void update_emote(Emote& emote, double delta_time) 
{
	if(!emote.animator.is_visible)
		return;

	iterate_animator(emote.animator, delta_time);
	if (emote.animator.is_looping) {
		emote.time_to_disappear -= delta_time;
		if (emote.time_to_disappear <= 0) {
			emote.animator.is_visible = false;
		}
	} else if (emote.animator.is_finished) {
		emote.animator.is_visible = false;
	}
}

void activate_emote(Emote& emote, Sequence* sequence, double length) 
{
	emote.animator.is_visible = true;
	emote.animator.sequence = sequence;
	emote.animator.is_looping = true;
	emote.animator.frame = 0;
	emote.time_to_disappear = length;
}