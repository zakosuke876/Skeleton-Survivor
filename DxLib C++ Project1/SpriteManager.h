#pragma once
#include <array>
#include <vector>
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

	// 1アニメーション用のデータ構造体
	struct AnimationData {
		const char* path; // 読み込むパス
		int frameCount;   // 分割総数
		int splitX;       // 横方向の分割数
		int splitY;       // 縦方向の分割数
		int width;        // 横幅
		int height;       // 縦幅
		float frameTime;    // 1コマの再生時間
		std::vector<int> handles; // データを格納する配列
	};

	// 全アニメーション情報をEffectTypeで引くテーブル
	// EffectTypeの宣言順と行を合わせる
	std::array<AnimationData, static_cast<int>(EffectType::Count)> animations = { {
		{"Textures/Effect/Text/LevelUp.png", 43, 6, 8, 320, 128, 0.03f, {}},
		{"Textures/Effect/Death/PlayerDeath.png",  50, 50, 1, 64, 64, 0.035f, {}},
		{"Textures/Effect/Death/EnemyDeath.png", 12, 12, 1, 64, 64, 0.1f, {}},
		{"Textures/Effect/Revive/Revive.png", 29, 29, 1, 128, 128, 0.07f, {}},
		{"Textures/Effect/Hit/PlayerAttackHit.png", 7, 7, 1, 96, 96, 0.1f, {}},
	} };
};
