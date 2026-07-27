#pragma once
#include "Enemy.h"
#include "EnemyWeapon.h"


// SKELETON_MAGE独自のステート,ENEMY_STATE_MAXからの続き番号
enum Skeleton_MageState {

	SKELETON_M_VICTORY = ENEMY_STATE_MAX       // 勝利状態
};

// アニメーションテーブルはSkeleton_Mage.cppで定義;
extern const EnemyAnimData SKELETON_MAGE_ANIM_TABLE[];

// 前方宣言
class MagicEffectManager;

class Skeleton_Mage : public Enemy {
public:

	Skeleton_Mage(float startX, float startY, float startZ, int skeletonNModel, int TexHandle, SkeletonMageStatus& mageData, int staffModel, int staffTexHandle, float growRate);
	~Skeleton_Mage() override = default;
	void Update(Player& player, float deltaTime, MagicEffectManager& magicEffectManager) override;
	EnemyType GetEnemyType() const override { return enemyType; }

private:

	void UpdateSpawn() override;
	void UpdateAttack(Player& player, float deltaTime, MagicEffectManager& magicEffectManager) override;
	void UpdateDamage() override;
	void UpdateDeath(float deltaTime) override;
	void DrawModel() const override;

	// 杖
	EnemyWeapon staff;

	// 今回の攻撃で発射したか
	bool hasFired = false;

	// 攻撃アニメーションのどの時点で発射するか(0.0～1.0)
	static constexpr float FIRE_TIMING = 0.6f;

	// 発射位置の高さオフセット
	static constexpr float FIRE_OFFSET = 20.0f;
};