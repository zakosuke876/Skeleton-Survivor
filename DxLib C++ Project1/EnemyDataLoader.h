#pragma once
#include "Enemy.h"
#include "EnemyStatusTypes.h"
#include "EnemySpawnConfig.h"
#include <string>

class EnemyDataLoader {
public:

	// ¬Œ÷‚µ‚½‚çtrue‚ğ•Ô‚µAoutData‚ÉŒ‹‰Ê‚ğŠi”[‚·‚é
	//static bool LoadEnemyData(const char* jsonPath, const char* enemyKey, EnemyStatus& outData);
	//static bool LoadSkeletonWarriorData(const char* jsonPath, SkeletonWarriorStatus& outData);
	static bool LoadSpawnConfig(const char* jsonPath, EnemySpawnConfig& outData);

	bool LoadNormal(EnemyStatus& outData);
	bool LoadWarrior(EnemyStatus& outData);
	bool LoadData(const char* jsonPath, const char* enemyKey, EnemyStatus& outData);

	bool UniqueData();
	bool LoadWarriorUnique(SkeletonWarriorStatus& outData);

private:

};