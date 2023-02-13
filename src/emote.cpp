#include "emote.h"

Emote::Emote(Vec2* target_position, Vec2 offset_position, Sequence* sequence) 
: target_position(target_position), offset_position(offset_position)
{
	animator.is_looping = false;
	animator.sequence = sequence;
}

Emote::Emote(Vec2* target_position, Vec2 offset_position, Sequence* sequence, double length)
	: target_position(target_position), offset_position(offset_position)
{
	time_to_disappear = length;
	animator.is_looping = true;
	animator.sequence = sequence;
}

void update_emotes(std::vector<Emote>& emotes, double delta_time)
{
	std::vector<int> to_delete;
	for (int i = 0; i < emotes.size(); ++i) {
		iterate_animator(emotes[i].animator, delta_time);

		if (emotes[i].animator.is_looping) {
			emotes[i].time_to_disappear -= delta_time;
			if (emotes[i].time_to_disappear <= 0) {
				to_delete.push_back(i);
			}
		} else if (emotes[i].animator.is_finished) {
			to_delete.push_back(i);
		}
	}

	for (int i : to_delete) {
		emotes[i] = emotes.back();
		emotes.pop_back();
	}
}
