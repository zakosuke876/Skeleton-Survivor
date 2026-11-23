#pragma once

// エフェクトの種類
enum class EffectType {
	LevelUp,
	PlayerDeath,
	EnemyDeath,
	Revive,
	PlayerAttackHit, // 自機の攻撃が敵に当たった
	//EnemyAttackHit,  // 敵の近接が自機に当たった
	//MagicHit,        // 魔法が自機に当たった
	Count,
};

// 1エフェクト分の再生データ
struct AnimationClip {
	const int* handles = nullptr; //
	int handleNum = 0;            // 総コマ数
	float frameTime = 0.0f;       // 1コマの表示時間
};
