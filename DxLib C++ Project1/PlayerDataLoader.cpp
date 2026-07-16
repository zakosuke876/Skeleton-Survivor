#include "PlayerDataLoader.h"
#include "JsonPath.h"
#include <fstream>
#include "DxLib.h"

#include "include/json.hpp"
using json = nlohmann::json;

bool PlayerDataLoader::Load(PlayerStatus& outData) {

	return LoadData(JsonPath::PLAYER_DATA, outData);
}

bool PlayerDataLoader::LoadData(const char* jsonPath, PlayerStatus& outData) {

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

		// "Player"キーが存在しない場合は処理しない
		if (!j.contains("Player")) return false;

		const json& data = j["Player"];

		// 読み込んだ値を構造体へ設定
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
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}

	return true;
}