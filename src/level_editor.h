#pragma once
#include <unordered_map>
#include "level.h"

enum class LevelEditorEntityType {
	KING,
	TILE,
	ENEMY,
	FOOD
};

struct LevelEditorEntity {
	Vec2 position;
	LevelEditorEntityType type;
};

struct LevelEditor {
	LevelData* data;
	std::unordered_map<LevelEditorEntityType, Sequence*> type_to_sequence_map;
	std::vector<LevelEditorEntity> entities;
};

void load_editor(LevelEditor& level, Sequences& sequences, Sound& sound, Platform& platform);
void tick_editor(LevelEditor& editor, Platform& platform);
void draw_editor(LevelEditor& editor, int atlas, Platform& platform);
