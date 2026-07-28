#pragma once
#include "Enemy.h"
#include "EnemyStatusTypes.h"
#include "EnemySpawnConfig.h"
#include <string>

class EnemyDataLoader {
public:

	static bool LoadSpawnConfig(const char* jsonPath, EnemySpawnConfig& outData);

	bool LoadNormal(EnemyStatus& outData);
	bool LoadWarrior(EnemyStatus& outData);
	bool LoadData(const char* jsonPath, const char* enemyKey, EnemyStatus& outData);

private:

};