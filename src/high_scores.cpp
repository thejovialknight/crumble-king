#include "high_scores.h"

// TODO: Sort better, just loop through and add it at the correct index, obviously.
void add_high_score(std::vector<int>& scores, int new_score)
{
	scores.push_back(new_score);
	std::sort(scores.begin(), scores.end(), std::greater<int>());
}

void write_high_scores(std::vector<int>& scores, Platform& platform)
{
	std::string score_text;
	for (int i = 0; i < 10; ++i) {
		if (i < scores.size()) {
			score_text += std::to_string(scores[i]) + "\n";
		} else {
			score_text += "0\n";
		}
	}
    write_file_text("resources/save/scores.txt", score_text.c_str()); 
}
