#include "SpawnDataLoader.h"
#include "JsonPath.h"
#include "JsonLoader.h"
#include <fstream>
#include "DxLib.h"

#include "include/json.hpp"
using json = nlohmann::json;

bool SpawnDataLoader::Load(EnemySpawnConfig& outData) {

	return LoadData(JsonPath::SPAWN_DATA, outData);
}

bool SpawnDataLoader::LoadData(const char* jsonPath, EnemySpawnConfig& outData) {

	return JsonLoader::Load(jsonPath, "SpawnConfig", [&](const json& data) {

		outData.spawnInterval = data.at("spawnInterval").get<float>();

		outData.maxEnemy = data.at("maxEnemy").get<int>();

		outData.growInterval = data.at("growInterval").get<float>();

		outData.growRatePerInterval = data.at("growRatePerInterval").get<float>();

		return true;
		});
}