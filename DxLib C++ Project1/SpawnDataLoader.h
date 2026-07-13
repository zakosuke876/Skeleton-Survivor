#pragma once
#include "EnemySpawnConfig.h"

class SpawnDataLoader {
public:

	static bool Load(EnemySpawnConfig& outData);

private:

	static bool LoadData(const char* jsonPath, EnemySpawnConfig& outData);
};