#pragma once
#include "Enemy.h"

class Skeleton_Mage : public Enemy {
public:

	Skeleton_Mage(float startX, float startY, float startZ, int skeletonNModel, int TexHandle, SkeletonNormalStatus& normalData, float growRate);
	~Skeleton_Mage() override = default;
	void Update(Player& player, float deltaTime) override;
	EnemyType GetEnemyType() const override { return enemyType; }

private:

	void UpdateSpawn() override;
	void UpdateAttack(Player& player, float deltaTime);
	void UpdateDamage() override;
	void UpdateDeath(float deltaTime) override;
	void DrawModel() const override;
};