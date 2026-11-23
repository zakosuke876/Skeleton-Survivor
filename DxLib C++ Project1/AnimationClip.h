#pragma once

// エフェクトの種類
enum class EffectType {
	LevelUp,
	PlayerDeath,
	EnemyDeath,
	Revive,
	Attack,
	Count,
};

// 1エフェクト分の再生データ
struct AnimationClip {
	const int* handles = nullptr; //
	int handleNum = 0;            // 総コマ数
	float frameTime = 0.0f;       // 1コマの表示時間
};
