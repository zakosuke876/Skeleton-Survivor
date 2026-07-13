#pragma once
#include "EnemyStatusTypes.h"

class EnemyNormalDataLoader {
public:

	static bool Load(SkeletonNormalStatus& outData);

private:

	static bool LoadData(const char* jsonPath, SkeletonNormalStatus& outData);
};