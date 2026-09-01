#pragma once
#include "Enemy.h"

// Skeleton_Normal独自のアニメーション,ENEMY_ANIM_MAX,からの続き番号
enum Skeleton_NormalAnimation {

	SKELETON_N_ANIM_VICTORY_POSE = ENEMY_ANIM_MAX,  // 勝利アニメーション
};

// SKELETON_NORMAL独自のステート,ENEMY_STATE_MAXからの続き番号
enum Skeleton_NormalState {

	SKELETON_N_VICTORY = ENEMY_STATE_MAX       // 勝利状態
};

// アニメーションテーブルはSkeleton_Normal.cppで定義;
extern const EnemyAnimData SKELETON_NORMAL_ANIM_TABLE[];


// 前方宣言
class MagicEffectManager;


class Skeleton_Normal : public Enemy {
public:

	Skeleton_Normal(float startX, float startY, float startZ, int skeletonNModel, int TexHandle, SkeletonNormalStatus& normalData, float growRate);
	~Skeleton_Normal() override = default;
	void Update(Player& player, float deltaTime, MagicEffectManager& magicEffectManager) override;
	EnemyType GetEnemyType() const override { return enemyType; }

private:

	void UpdateSpawn() override;
	void UpdateAttack(Player& player, float deltaTime, MagicEffectManager& magicEffectManager) override;
	void UpdateDamage() override;
	void UpdateDeath(float deltaTime) override;
	void DrawModel() const override;
};