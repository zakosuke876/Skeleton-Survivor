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




// Loaderがデータ保持バージョン
/*bool PlayerDataLoader::Load() {

	return LoadData("Data/PlayerData.json");
}

bool PlayerDataLoader::LoadData(const char* jsonPath) {

	// JSONファイルを開く
	std::ifstream file(jsonPath);

	// ファイルが開けなかった場合失敗
	if (!file.is_open())
	{
		printfDx("ファイルが開けません: %s\n", jsonPath);
		return false;
	}

	// JSONデータを格納する変数
	json j;

	try
	{
		// JSONファイルの内容を読み込む
		file >> j;

		// "Player"キーが存在しない場合は処理しない
		if (!j.contains("Player")) return false;

		// 最大HPを取得
		playerDataTable.maxHp = j["Player"].at("maxHp").get<int>();

		// 攻撃力を取得
		playerDataTable.attackPower = j["Player"].at("attackPower").get<int>();

		// 歩きスピードを取得
		playerDataTable.speed = j["Player"].at("speed").get<float>();

		// ダッシュスピードの計算に使う値を取得
		playerDataTable.dashMultiplier = j["Player"].at("dashMultiplier").get<float>();

		// 初期レベルを取得
		playerDataTable.startLevel = j["Player"].at("startLevel").get<int>();

		// 次回レベルアップに必要な経験値量の初期値を取得
		playerDataTable.startNextLevelExp = j["Player"].at("startNextLevelExp").get<int>();

		// 攻撃判定の半径を取得
		playerDataTable.attackRadius = j["Player"].at("attackRadius").get<float>();

		// 体の当たり判定半径を取得
		playerDataTable.bodyRadius = j["Player"].at("bodyRadius").get<float>();

		// レベルアップ時に上昇する攻撃力を取得
		playerDataTable.levelUpAttackPower = j["Player"].at("levelUpAttackPower").get<int>();

		// レベルアップに必要な経験値の倍率を取得
		playerDataTable.nextLevelExpMultiplier = j["Player"].at("nextLevelExpMultiplier").get<float>();

		// 無敵時間を取得
		playerDataTable.invincibleTime = j["Player"].at("invincibleTime").get<float>();

		// 死亡後消えるまでの時間を取得
		playerDataTable.hideTime = j["Player"].at("hideTime").get<float>();

		// レベルアップ時のテキスト表示時間を取得
		playerDataTable.levelUpTextActiveTime = j["Player"].at("levelUpTextActiveTime").get<float>();

		// モデルの拡大倍率を取得
		playerDataTable.scale = j["Player"].at("scale").get<float>();

		// モデルのファイルパスを取得
		playerDataTable.modelPath = j["Player"].at("modelPath").get<std::string>();

		// テクスチャのファイルパスを取得
		playerDataTable.texturePath = j["Player"].at("texturePath").get<std::string>();
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}

	return true;
}*/