#include "EnemyDataLoader.h"
#include <fstream>

#include "include/json.hpp"
using json = nlohmann::json;

bool EnemyDataLoader::LoadEnemyData(const char* jsonPath, const char* enemyKey, EnemyStatus& outData) {

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

		// enemyKeyが存在しない場合は処理しない
		if (!j.contains(enemyKey)) return false;

		// 基礎HPを取得する
		outData.baseHp = j[enemyKey].at("baseHp").get<int>();

		// 基礎攻撃力を取得する
		outData.baseAttack = j[enemyKey].at("baseAttack").get<int>();

		// 歩きスピードを取得
		outData.speed = j[enemyKey].at("speed").get<float>();

		// ダッシュスピードを取得
		outData.dashSpeed = j[enemyKey].at("dashSpeed").get<float>();

		// 攻撃クールタイムを取得
		outData.attackCoolTime = j[enemyKey].at("attackCoolTime").get<float>();

		// 無敵時間を取得
		outData.invincibleTime = j[enemyKey].at("invincibleTime").get<float>();

		// 死亡後消えるまでの時間を取得
		outData.hideTime = j[enemyKey].at("hideTime").get<float>();

		// チェイス状態への遷移距離を取得
		outData.chaseRangeRadius = j[enemyKey].at("chaseRangeRadius").get<float>();

		// 攻撃状態への遷移距離を取得
		outData.attackRangeRadius = j[enemyKey].at("attackRangeRadius").get<float>();

		// 攻撃判定の半径を取得
		outData.attackRadius = j[enemyKey].at("attackRadius").get<float>();

		// 体の当たり判定半径を取得
		outData.bodyRadius = j[enemyKey].at("bodyRadius").get<float>();

		// スコアを取得
		outData.score = j[enemyKey].at("score").get<int>();

		// モデルの拡大倍率を取得
		outData.scale = j[enemyKey].at("scale").get<float>();

		// モデルのファイルパスを取得
		outData.modelPath = j[enemyKey].at("modelPath").get<std::string>();

		// テクスチャのファイルパスを取得
		outData.texturePath = j[enemyKey].at("texturePath").get<std::string>();
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}

	return true;
}

bool EnemyDataLoader::LoadSkeletonWarriorData(const char* jsonPath, SkeletonWarriorStatus& outData) {

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

		// Skeleton_Warriorが存在しない場合は処理しない
		if (!j.contains("Skeleton_Warrior")) return false;

		// 復活確率を取得
		outData.reviveChancePercent = j["Skeleton_Warrior"].at("reviveChancePercent").get<int>();

		// 復活待機時間を取得
		outData.reviveWaitTime = j["Skeleton_Warrior"].at("reviveWaitTime").get<float>();

		// 復活時のHP倍率を取得
		outData.reviveHpMultiplier = j["Skeleton_Warrior"].at("reviveHpMultiplier").get<float>();

		// 復活時のスコア倍率を取得
		outData.reviveScoreMultiplier = j["Skeleton_Warrior"].at("reviveScoreMultiplier").get<int>();
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}

	return true;
}

bool EnemyDataLoader::LoadSpawnConfig(const char* jsonPath, EnemySpawnConfig& outData) {

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

		// SpawnConfigが存在しない場合は処理しない
		if (!j.contains("SpawnConfig")) return false;

		// スポーン間隔を取得
		outData.spawnInterval = j["SpawnConfig"].at("spawnInterval").get<float>();

		// 敵の最大同時出現数を取得
		outData.maxEnemy = j["SpawnConfig"].at("maxEnemy").get<int>();

		// 敵が強化される間隔を取得
		outData.growInterval = j["SpawnConfig"].at("growInterval").get<float>();

		// 1回の強化で上昇する倍率を取得
		outData.growRatePerInterval = j["SpawnConfig"].at("growRatePerInterval").get<float>();
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}

	return true;
}