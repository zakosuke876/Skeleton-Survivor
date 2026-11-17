#include "SpriteManager.h"
#include "DxLib.h"

SpriteManager::SpriteManager() {

	// 全要素を-1で初期化
	levelUpTextHandles.fill(-1);

	LoadDivGraph(LEVEL_UP_TEXT_PATH, LEVEL_UP_ALL, LEVEL_UP_DIV_X, LEVEL_UP_DIV_Y, LEVEL_UP_WIDTH, LEVEL_UP_HEIGHT, levelUpTextHandles.data());
}

SpriteManager::~SpriteManager() {

	for (int i = 0; i < LEVEL_UP_ALL; i++)
	{
		if (levelUpTextHandles[i] != -1) DeleteGraph(levelUpTextHandles[i]);
	}
}
