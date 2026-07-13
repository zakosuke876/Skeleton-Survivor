#include "SpawnDataLoader.h"
#include "JsonPath.h"
#include <fstream>
#include "DxLib.h"

#include "include/json.hpp"
using json = nlohmann::json;

bool SpawnDataLoader::Load(EnemySpawnConfig& outData) {

	return LoadData(JsonPath::SPAWN_DATA, outData);
}

bool SpawnDataLoader::LoadData(const char* jsonPath, EnemySpawnConfig& outData) {

	std::ifstream file(jsonPath);

	// ファイルが開けなかった場合失敗
	if (!file.is_open())
	{
		printfDx("ファイルが開けません: %s\n", jsonPath);
		return false;
	}

	json j;

	try
	{
		file >> j;

		// "SpawnConfig"キーが存在しない場合は処理しない
		if (!j.contains("SpawnConfig")) return false;

		const json& data = j["SpawnConfig"];

		outData.spawnInterval = data.at("spawnInterval").get<float>();

		outData.maxEnemy = data.at("maxEnemy").get<int>();

		outData.growInterval = data.at("growInterval").get<float>();

		outData.growRatePerInterval = data.at("growRatePerInterval").get<float>();
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}

	return true;
}