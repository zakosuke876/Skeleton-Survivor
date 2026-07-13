#pragma once
#include "Enemy.h"
#include "EnemySpawnConfig.h"
#include <random>
#include <vector>

// 前方宣言
class Player;
class ScoreManager;
class ItemManager;
class SoundManager;



class EnemyManager {
public:

	EnemyManager();
	~EnemyManager();
	void Reset(); // ゲームリセット処理
	void Update(Player& player, float deltaTime);
	void Draw() const;
	void DrawUI() const;
	void CheckPlayerAttackHit(const Player& player, ScoreManager& scoreManager, ItemManager& itemManager, SoundManager& soundManager);
	void CheckEnemyAttackHit(Player& player, SoundManager& soundManager);

	//	倒した敵の数を敵の種類ごとにカウント
	void CountEnemyKill(EnemyType type);

	// 倒した敵の数を種類別で取得するゲッター
	int GetKillCount(EnemyType type) const {

		return killCount[type];
	}

private:

	void Spawn();

	// 敵管理
	std::vector<std::unique_ptr<Enemy>> enemies;
	float spawnTimer = 0.0f;

	// ノーマルスケルトンのステータス
	SkeletonNormalStatus normalStatus;

	// ウォーリアースケルトンのステータス
	SkeletonWarriorStatus  warriorStatus;

	// スポーン関係のデータ
	EnemySpawnConfig spawnConfig;
	
	// JSONから読み込む値
	// 敵のスポーン間隔(秒)
	float spawnInterval = -1.0f;

	// 敵の最大同時出現数
	int maxEnemy = -1;

	// 敵が強化される間隔(秒)
	float growInterval = -1.0f;

	// 1回の強化で上昇する倍率
	float growRatePerInterval = -1.0f;

	// 時間経過ごとに敵を強化する
	float playTimer = 0.0f;

	// 敵のステータス強化倍率
	float enemyGrowRate = 1.0f;

	// 倒した敵の数を管理する配列
	int killCount[ENEMY_TYPE_COUNT] = {0};

	// Skeleton_Normalの複製元モデル
	int skeletonNormalModelHandle  = -1;

	// Skeleton_Warriorの複製元モデル
	int skeletonWarriorModelHandle = -1;

	// Skeleton_Normal用テクスチャ
	int skeletonNormalTexHandle = -1;

	// Skeleton_Warrior用テクスチャ
	int skeletonWarriorTexHandle = -1;

	// ランダムエンジン
	std::mt19937 rng;
};