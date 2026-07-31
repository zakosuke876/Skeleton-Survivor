#include "EnemyMageDataLoader.h"
#include "EnemyStatusTypes.h"
#include "JsonPath.h"
#include "JsonLoader.h"

#include "include/json.hpp"
using json = nlohmann::json;

bool EnemyMageDataLoader::Load(SkeletonMageStatus& outData) {

	// JsonPath・構造体を渡す
	return LoadData(JsonPath::SKELETON_MAGE_DATA, outData);
}

bool EnemyMageDataLoader::LoadData(const char* jsonPath, SkeletonMageStatus& outData) {

	return JsonLoader::Load(jsonPath, "Skeleton_Mage", [&](const json& data) {

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

		return true;
	});
}