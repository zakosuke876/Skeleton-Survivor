#include "EnemyNormalDataLoader.h"
#include "EnemyStatusTypes.h"
#include "JsonPath.h"
#include <fstream>
#include "DxLib.h"

#include "include/json.hpp"
using json = nlohmann::json;

bool EnemyNormalDataLoader::Load(SkeletonNormalStatus& outData) {

	return LoadData(JsonPath::SKELETON_NORMAL_DATA, outData);
}

bool EnemyNormalDataLoader::LoadData(const char* jsonPath, SkeletonNormalStatus& outData) {

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

		// "Skeleton_Normal"キーが存在しない場合は処理しない
		if (!j.contains("Skeleton_Normal")) return false;

		const json& data = j["Skeleton_Normal"];

		// 読み込んだ値を構造体へ設定
		outData.enemyStatus.baseHp = data.at("baseHp").get<int>();

		outData.enemyStatus.baseAttack = data.at("baseAttack").get<int>();

		outData.enemyStatus.speed = data.at("speed").get<float>();

		outData.enemyStatus.dashSpeed = data.at("dashSpeed").get<float>();

		outData.enemyStatus.attackCoolTime = data.at("attackCoolTime").get<float>();

		outData.enemyStatus.invincibleTime = data.at("invincibleTime").get<float>();

		outData.enemyStatus.hideTime = data.at("hideTime").get<float>();

		outData.enemyStatus.chaseRangeRadius = data.at("chaseRangeRadius").get<float>();

		outData.enemyStatus.attackRangeRadius = data.at("attackRangeRadius").get<float>();

		outData.enemyStatus.attackRadius = data.at("attackRadius").get<float>();

		outData.enemyStatus.bodyRadius = data.at("bodyRadius").get<float>();

		outData.enemyStatus.score = data.at("score").get<int>();

		outData.enemyStatus.scale = data.at("scale").get<float>();

		outData.enemyStatus.modelPath = data.at("modelPath").get<std::string>();

		outData.enemyStatus.texturePath = data.at("texturePath").get<std::string>();
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}

	return true;
}