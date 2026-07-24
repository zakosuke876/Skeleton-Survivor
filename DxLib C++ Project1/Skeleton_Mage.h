#pragma once
#include "Enemy.h"


// アニメーションテーブルはSkeleton_Mage.cppで定義;
extern const EnemyAnimData SKELETON_MAGE_ANIM_TABLE[];

// 前方宣言
class MagicEffectManager;

class Skeleton_Mage : public Enemy {
public:

	Skeleton_Mage(float startX, float startY, float startZ, int skeletonNModel, int TexHandle, SkeletonMageStatus& mageData, float growRate);
	~Skeleton_Mage() override = default;
	void Update(Player& player, float deltaTime, MagicEffectManager& magicEffectManager) override;
	EnemyType GetEnemyType() const override { return enemyType; }

private:

	void UpdateSpawn() override;
	void UpdateAttack(Player& player, float deltaTime, MagicEffectManager& magicEffectManager) override;
	void UpdateDamage() override;
	void UpdateDeath(float deltaTime) override;
	void DrawModel() const override;

	// 今回の攻撃で発射したか
	bool hasFired = false;

	// 攻撃アニメーションのどの時点で発射するか(0.0～1.0)
	static constexpr float FIRE_TIMING = 0.6f;

	// 発射位置の高さオフセット
	static constexpr float FIRE_OFFSET = 20.0f;
};