#include "animation.h"

void iterate_animator(Animator& animator, double delta_time)
{
	animator.time_till_next_frame -= delta_time;
	if (animator.time_till_next_frame <= 0) {
		animator.time_till_next_frame = animator.frame_length;
		animator.frame++;
	}

	// Loop iteration outside of first if statement in case animation is changed to one with less frames
	if (animator.frame >= animator.sequence->frames.size()) {
		if(animator.is_looping) {
			animator.frame = 0;
		} else {
			animator.frame--;
			animator.is_finished = true;
		}
	}
}

PlatformSprite sprite_from_animator(int atlas, Animator& animator, const Vec2& position)
{
	if(!animator.is_visible) {
		return PlatformSprite(
			atlas,
			IRect(),
			0,
			0,
			0,
			0,
			false
		);
	}
	return sprite_from_sequence(atlas, *animator.sequence, animator.frame, position, animator.is_flipped);
}

PlatformSprite sprite_from_sequence(int atlas, const Sequence& sequence, int frame, const Vec2& position, bool is_flipped)
{
	return PlatformSprite(
		atlas,
		sequence.frames[frame],
		(int)position.x,
		(int)position.y,
		(int)sequence.origin.x,
		(int)sequence.origin.y,
		is_flipped
	);
}

void populate_sequences(const std::string text, Sequences& sequences)
{
	for(int i = 0; i < text.length(); ++i) {
		if(!try_iterate_past_char('@', text, i)) break;
		std::string sequence_name = pull_string_before_char(',', text, i);
		Sequence sequence;
		// TODO: stod? int? what?
		int width = std::stod(pull_string_before_char(',', text, i));
		int height = std::stod(pull_string_before_char(',', text, i));
		sequence.origin.x = std::stod(pull_string_before_char(',', text, i));
		sequence.origin.y = std::stod(pull_string_before_char(':', text, i));
		while(text[i] != '\n' && text[i] != '\0') {
			int pos_x = pull_int_before_char(',', text, i);
			int pos_y = pull_int_before_char(';', text, i);
			sequence.frames.emplace_back(IRect(
				pos_x,
				pos_y,
				width,
				height
			));
		}
		
		if(sequence_name == "TILE_ISLAND") { sequences.tile_island = sequence; }
		if(sequence_name == "TILE_CENTER") { sequences.tile_center = sequence; }
		if(sequence_name == "TILE_LEFT") { sequences.tile_left = sequence; }
		if(sequence_name == "TILE_RIGHT") { sequences.tile_right = sequence; }
		if(sequence_name == "BUBBLING_POT") { sequences.bubbling_pot = sequence; }
		if(sequence_name == "PLATTER") { sequences.platter = sequence; }
		if(sequence_name == "FOOD_POTATO") { sequences.food_potato = sequence; }
		if(sequence_name == "FOOD_WATERMELON") { sequences.food_watermelon = sequence; }
		if(sequence_name == "FOOD_GRAPE") { sequences.food_grape = sequence; }
		if(sequence_name == "FOOD_CHICKEN") { sequences.food_chicken = sequence; }
		if(sequence_name == "KING_IDLE") { sequences.king_idle = sequence; }
		if(sequence_name == "KING_RUN") { sequences.king_run = sequence; }
		if(sequence_name == "KING_JUMP") { sequences.king_jump = sequence; }
		if(sequence_name == "KING_FLOAT") { sequences.king_float = sequence; }
		if(sequence_name == "GUARD_IDLE") { sequences.guard_idle = sequence; }
		if(sequence_name == "GUARD_RUN") { sequences.guard_run = sequence; }
		if(sequence_name == "GUARD_JUMP") { sequences.guard_jump = sequence; }
		if(sequence_name == "GUARD_END") { sequences.guard_end = sequence; }
		if(sequence_name == "WINDOW") { sequences.window = sequence; }
	}
}

