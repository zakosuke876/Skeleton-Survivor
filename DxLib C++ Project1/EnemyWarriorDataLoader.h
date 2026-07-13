#pragma once
#include "EnemyStatusTypes.h"

class EnemyWarriorDataLoader {
public:

	static bool Load(SkeletonWarriorStatus& outData);

private:

	static bool LoadData(const char* jsonPath, SkeletonWarriorStatus& outData);
};