#include "EnemyNormalDataLoader.h"
#include "EnemyStatusTypes.h"
#include "JsonPath.h"
#include "JsonLoader.h"

#include "include/json.hpp"
using json = nlohmann::json;

bool EnemyNormalDataLoader::Load(SkeletonNormalStatus& outData) {

	return LoadData(JsonPath::SKELETON_NORMAL_DATA, outData);
}

bool EnemyNormalDataLoader::LoadData(const char* jsonPath, SkeletonNormalStatus& outData) {

	return JsonLoader::Load(jsonPath, "Skeleton_Normal", [&](const json& data) {

		// ì«Ç›çûÇÒÇæílÇç\ë¢ëÃÇ÷ê›íË
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