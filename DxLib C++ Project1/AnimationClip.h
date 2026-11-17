#pragma once

// エフェクトの種類
enum class EffectType {
	Attack,
	Death,
	LevelUp,
	Count,
};

// 1アニメ分の再生データ
struct AnimationClip {
	const int* handles = nullptr; //
	int handleNum = 0;            // 総コマ数
	float frameTime = 0.0f;       // 1コマの表示時間
};
