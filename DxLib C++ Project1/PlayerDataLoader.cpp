#include "PlayerDataLoader.h"
#include <fstream>

#include "include/json.hpp"
using json = nlohmann::json;

bool PlayerDataLoader::LoadPlayerData(const char* jsonPath, PlayerStatus& outData) {

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
		outData.maxHp = j["Player"].at("maxHp").get<int>();

		// 攻撃力を取得
		outData.attackPower = j["Player"].at("attackPower").get<int>();

		// 歩きスピードを取得
		outData.speed = j["Player"].at("speed").get<float>();

		// ダッシュスピードの計算に使う値を取得
		outData.dashMultiplier = j["Player"].at("dashMultiplier").get<float>();

		// 初期レベルを取得
		outData.startLevel = j["Player"].at("startLevel").get<int>();

		// 次回レベルアップに必要な経験値量の初期値を取得
		outData.startNextLevelExp = j["Player"].at("startNextLevelExp").get<int>();

		// 攻撃判定の半径を取得
		outData.attackRadius = j["Player"].at("attackRadius").get<float>();

		// 体の当たり判定半径を取得
		outData.bodyRadius = j["Player"].at("bodyRadius").get<float>();

		// レベルアップ時に上昇する攻撃力を取得
		outData.levelUpAttackPower = j["Player"].at("levelUpAttackPower").get<int>();

		// レベルアップに必要な経験値の倍率を取得
		outData.nextLevelExpMultiplier = j["Player"].at("nextLevelExpMultiplier").get<float>();

		// 無敵時間を取得
		outData.invincibleTime = j["Player"].at("invincibleTime").get<float>();

		// 死亡後消えるまでの時間を取得
		outData.hideTime = j["Player"].at("hideTime").get<float>();

		// レベルアップ時のテキスト表示時間を取得
		outData.levelUpTextActiveTime = j["Player"].at("levelUpTextActiveTime").get<float>();

		// モデルの拡大倍率を取得
		outData.scale = j["Player"].at("scale").get<float>();

		// モデルのファイルパスを取得
		outData.modelPath = j["Player"].at("modelPath").get<std::string>();

		// テクスチャのファイルパスを取得
		outData.texturePath = j["Player"].at("texturePath").get<std::string>();
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}

	return true;
}