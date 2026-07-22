#pragma once
#include "EnemyStatusTypes.h"

class EnemyWarriorDataLoader {
public:

	static bool Load(SkeletonWarriorStatus& outData);

	static bool Load(SkeletonNormalStatus& outData);


	//static bool Load(const char* enemyKey);

private:

	static bool LoadData(const char* jsonPath, SkeletonWarriorStatus& outData);
};