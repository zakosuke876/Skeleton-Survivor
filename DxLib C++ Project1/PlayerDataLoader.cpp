#include "PlayerDataLoader.h"
#include "JsonPath.h"
#include "JsonLoader.h"

#include "include/json.hpp"
using json = nlohmann::json;

bool PlayerDataLoader::Load(PlayerStatus& outData) {

	return LoadData(JsonPath::PLAYER_DATA, outData);
}

bool PlayerDataLoader::LoadData(const char* jsonPath, PlayerStatus& outData) {

	return JsonLoader::Load(jsonPath, "Player", [&](const json& data) {

		// ì«Ç›çûÇÒÇæílÇç\ë¢ëÃÇ÷ê›íË
		outData.maxHp = data.at("maxHp").get<int>();

		outData.attackPower = data.at("attackPower").get<int>();

		outData.speed = data.at("speed").get<float>();

		outData.dashMultiplier = data.at("dashMultiplier").get<float>();

		outData.startLevel = data.at("startLevel").get<int>();

		outData.startNextLevelExp = data.at("startNextLevelExp").get<int>();

		outData.attackRadius = data.at("attackRadius").get<float>();

		outData.bodyRadius = data.at("bodyRadius").get<float>();

		outData.levelUpAttackPower = data.at("levelUpAttackPower").get<int>();

		outData.nextLevelExpMultiplier = data.at("nextLevelExpMultiplier").get<float>();

		outData.invincibleTime = data.at("invincibleTime").get<float>();

		outData.hideTime = data.at("hideTime").get<float>();

		outData.levelUpTextActiveTime = data.at("levelUpTextActiveTime").get<float>();

		outData.scale = data.at("scale").get<float>();

		outData.modelPath = data.at("modelPath").get<std::string>();

		outData.texturePath = data.at("texturePath").get<std::string>();

		return true;

	});
}