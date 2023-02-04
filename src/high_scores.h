#pragma once
#include "platform.h"
#include <algorithm>

void add_high_score(std::vector<int>& scores, int new_score);
void write_high_scores(std::vector<int>& scores, Platform& platform);
