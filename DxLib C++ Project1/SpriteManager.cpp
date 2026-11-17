#include "SpriteManager.h"
#include "Logger.h"
#include "DxLib.h"

SpriteManager::SpriteManager() {

	// 全要素を-1で初期化
	levelUpTextHandles.fill(-1);

	if (LoadDivGraph(LEVEL_UP_TEXT_PATH, LEVEL_UP_ALL, LEVEL_UP_DIV_X, LEVEL_UP_DIV_Y, LEVEL_UP_WIDTH, LEVEL_UP_HEIGHT, levelUpTextHandles.data()) == -1) WriteLog(LogLevel::Error, "LevelUpスプライトシートの読み込みに失敗しました");
}

SpriteManager::~SpriteManager() {

	for (int i = 0; i < LEVEL_UP_ALL; i++)
	{
		if (levelUpTextHandles[i] != -1) DeleteGraph(levelUpTextHandles[i]);
	}
}

AnimationClip SpriteManager::GetClip(EffectType type) const {
	
	switch (type)
	{
		case EffectType::LevelUp:
		{
			AnimationClip clip = {
			levelUpTextHandles.data(), // 画像ハンドル
			LEVEL_UP_ALL,              // 総コマ数
			LEVEL_UP_TEXT_FRAME_TIME,  // 1コマの再生時間
			};

			return clip;
		}

		default:
			return {};
	}
}
