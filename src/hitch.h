#pragma once

struct HitchInfo {
	bool should_hitch;
	double length;

	HitchInfo(bool should_hitch, double length) : should_hitch(should_hitch), length(length) {}
};
