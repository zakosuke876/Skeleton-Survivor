#pragma once
#include <array>
#include "AnimationClip.h"

class SpriteManager {
public:

	SpriteManager();
	~SpriteManager();

	// コピー禁止
	SpriteManager(const SpriteManager&) = delete;
	SpriteManager& operator = (const SpriteManager&) = delete;

	AnimationClip GetClip(EffectType type) const;

private:

	// 読み込むパス
	static constexpr const char* LEVEL_UP_TEXT_PATH = "Textures/Effect/Text/LevelUp.png";

	// 1コマの表示時間
	static constexpr float LEVEL_UP_TEXT_FRAME_TIME = 0.03f;

	// スプライトシートの総コマ数
	static constexpr int LEVEL_UP_ALL = 43;

	// 列
	static constexpr int LEVEL_UP_DIV_X = 6;

	// 行
	static constexpr int LEVEL_UP_DIV_Y = 8;

	// 横幅
	static constexpr int LEVEL_UP_WIDTH = 320;

	// 縦幅
	static constexpr int LEVEL_UP_HEIGHT = 128;

	// 画像ハンドル
	std::array<int, LEVEL_UP_ALL> levelUpTextHandles{};
};
