#include "EnemyDataLoader.h"
#include "EnemyStatusTypes.h"
#include "JsonLoader.h"
#include "JsonPath.h"
#include "include/json.hpp"

using json = nlohmann::json;

namespace EnemyDataLoader {

	bool LoadNormal(SkeletonNormalStatus& outData) {

		return JsonLoader::Load(JsonPath::SKELETON_NORMAL_DATA, "Skeleton_Normal", [&](const json& data) {

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

	bool LoadWarrior(SkeletonWarriorStatus& outData) {

		return JsonLoader::Load(JsonPath::SKELETON_WARRIOR_DATA, "Skeleton_Warrior", [&](const json& data) {

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

			// ウォーリアー固有ステータス
			outData.reviveChancePercent = data.at("reviveChancePercent").get<int>();

			outData.reviveWaitTime = data.at("reviveWaitTime").get<float>();

			outData.reviveHpMultiplier = data.at("reviveHpMultiplier").get<float>();

			outData.reviveScoreMultiplier = data.at("reviveScoreMultiplier").get<int>();

			return true;
			});
	}

	bool LoadMage(SkeletonMageStatus& outData) {

		return JsonLoader::Load(JsonPath::SKELETON_MAGE_DATA, "Skeleton_Mage", [&](const json& data) {

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
}
