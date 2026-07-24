#pragma once
#include "Enemy.h"

// Skeleton_Warrior独自のアニメーション,ENEMY_ANIM_MAX,からの続き番号
enum Skeleton_WarriorAnimation {

	SKELETON_W_REVIVE = ENEMY_ANIM_MAX, // 復活アニメーション
};

// SKELETON_WARRIOR独自のステート,ENEMY_STATE_MAXからの続き番号
enum Skeleton_WarriorState {

	SKELETON_W_VICTORY = ENEMY_STATE_MAX, // 勝利状態
	SKELETON_W_RESURRECTION,              // 復活状態
};

// アニメーションテーブルはSkeleton_Warrior.cppで定義;
extern const EnemyAnimData SKELETON_WARRIOR_ANIM_TABLE[];

// 前方宣言
class SoundManager;

class MagicEffectManager;


class Skeleton_Warrior : public Enemy {
public:

	Skeleton_Warrior(float startX, float startY, float startZ, int skeletonWModel, int TexHandle, SkeletonWarriorStatus& warriorData, float growRate);
	~Skeleton_Warrior() override = default;
	void Update(Player& player, float deltaTime, MagicEffectManager& magicEffectManager) override;
	bool TakeDamage(int value, SoundManager& soundManager) override;
	EnemyType GetEnemyType() const override { return enemyType; }

private:

	void UpdateSpawn() override;
	void UpdateAttack(Player& player, float deltaTime, MagicEffectManager& magicEffectManager);
	void UpdateDamage() override;
	void UpdateDeath(float deltaTime) override;
	void UpdateRevive();
	void DrawModel() const override;


	// JSONから読み込む値
	// 復活確率(%)
	int reviveChancePercent = -1;

	// 復活待機時間(秒)
	float reviveWaitTime = -1.0f;

	// 復活した際のHP倍率
	float reviveHpMultiplier = -1.0f;

	// 復活した際のスコア倍率
	int reviveScoreMultiplier = -1;


	// この敵が復活するかどうかの判定に使う乱数範囲
	static constexpr int REVIVE_CHANCE_MIN = 1;
	static constexpr int REVIVE_CHANCE_MAX = 100;

	// 復活したことがあるか
	bool isRevived = false;

	// 復活するかどうか
	bool willRevive = false;
};