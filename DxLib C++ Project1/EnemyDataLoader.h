#pragma once

// 前方宣言
struct SkeletonNormalStatus;
struct SkeletonWarriorStatus;
struct SkeletonMageStatus;

namespace EnemyDataLoader {

	bool LoadNormal(SkeletonNormalStatus& outData);
	bool LoadWarrior(SkeletonWarriorStatus& outData);
	bool LoadMage(SkeletonMageStatus& outData);
}
