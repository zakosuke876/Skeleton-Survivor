#include "EnemyManager.h"
#include "ScoreManager.h"
#include "ItemManager.h"
#include "SoundManager.h"
#include "Player.h"
#include "Skeleton_Normal.h"
#include "Skeleton_Warrior.h"
#include "EnemyNormalDataLoader.h"
#include "EnemyWarriorDataLoader.h"
#include "GameConfig.h"
#include <algorithm>
#include <cmath>
#include "DxLib.h"

EnemyManager::EnemyManager() {

	// ランダム生成用
	std::random_device rd;
	rng = std::mt19937(rd());

	// JSONから敵のスポーン関係の情報を取得
	/*EnemySpawnConfig spawnConfig;
	if (!EnemyDataLoader::LoadSpawnConfig("Data/SpawnConfig.json", spawnConfig))
	{
		printfDx("SpawnConfigのデータ読み込みに失敗しました\n");
	}*/

	/*spawnInterval = spawnConfig.spawnInterval;

	maxEnemy = spawnConfig.maxEnemy;

	growInterval = spawnConfig.growInterval;

	growRatePerInterval = spawnConfig.growRatePerInterval;*/

	/*// JSONからSkeleton_Normalのモデル・テクスチャパスを取得
	if (!EnemyDataLoader::LoadEnemyData("Data/SkeletonNormalData.json", "Skeleton_Normal", normalStatus))
	{
		printfDx("Skeleton_Normalのデータ読み込みに失敗しました\n");
	}

	// JSONからSkeleton_Warriorのモデル・テクスチャパスを取得
	if (!EnemyDataLoader::LoadEnemyData("Data/SkeletonWarriorData.json", "Skeleton_Warrior", warriorStatus))
	{
		printfDx("Skeleton_Warriorのデータ読み込みに失敗しました\n");
	}*/

	// JSONからSkeleton_Normalのモデル・テクスチャパスを取得
	if (!EnemyNormalDataLoader::Load(normalStatus))
	{
		printfDx("Skeleton_Normalのデータ読み込みに失敗しました\n");
	}

	// JSONからSkeleton_Warriorのモデル・テクスチャパスを取得
	if (!EnemyWarriorDataLoader::Load(warriorStatus))
	{
		printfDx("Skeleton_Warriorのデータ読み込みに失敗しました\n");
	}

	// モデルを読み込む
	skeletonNormalModelHandle = MV1LoadModel(normalStatus.enemyStatus.modelPath.c_str());
	skeletonWarriorModelHandle = MV1LoadModel(warriorStatus.enemyStatus.modelPath.c_str());

	// テクスチャを読み込む
	skeletonNormalTexHandle = LoadGraph(normalStatus.enemyStatus.texturePath.c_str());
	skeletonWarriorTexHandle = LoadGraph(warriorStatus.enemyStatus.texturePath.c_str());
}

EnemyManager::~EnemyManager() {

	// リソース開放
	MV1DeleteModel(skeletonNormalModelHandle);
	MV1DeleteModel(skeletonWarriorModelHandle);
	DeleteGraph(skeletonNormalTexHandle);
	DeleteGraph(skeletonWarriorTexHandle);
}

void EnemyManager::Reset() {

	enemies.clear();
	playTimer = 0.0f;
	spawnTimer = 0.0f;
	enemyGrowRate = 1.0f;

	// 配列の値を0にリセット
	for (int i = 0; i < ENEMY_TYPE_COUNT; i++)
	{
		killCount[i] = 0;
	}
}

void EnemyManager::Spawn() {

	// スポーンX,Z座標の範囲設定
	std::uniform_real_distribution<float> rx(GameConfig::FIELD_MIN_X, GameConfig::FIELD_MAX_X);
	std::uniform_real_distribution<float> rz(GameConfig::FIELD_MIN_Z, GameConfig::FIELD_MAX_Z);

	// 生成範囲を末尾-1にして敵を追加した際に自動で最大値に対応
	std::uniform_int_distribution<int> TypeNum(ENEMY_NORMAL, ENEMY_TYPE_COUNT - 1);

	// フィールド内ランダムな座標にスポーン
	float x = rx(rng);
	float z = rz(rng);

	int enemyTypeNum = TypeNum(rng);

	// 値によって生成する敵の種類を変える
	switch (enemyTypeNum)
	{
		case ENEMY_NORMAL:

			// 敵を生成
			enemies.emplace_back(std::make_unique<Skeleton_Normal>(x, 0.0f, z, skeletonNormalModelHandle, skeletonNormalTexHandle, normalStatus, enemyGrowRate));

			break;

		case ENEMY_WARRIOR:

			enemies.emplace_back(std::make_unique<Skeleton_Warrior>(x, 0.0f, z, skeletonWarriorModelHandle, skeletonWarriorTexHandle, warriorStatus, enemyGrowRate));

			break;
	}
}

void EnemyManager::Update(Player& player, float deltaTime) {

	// 削除前の敵の数を保持
	size_t enemyPrevCount = enemies.size();

	//	敵の数が上限未満の場合タイマーを進める
	if (enemies.size() < maxEnemy)
	{
		spawnTimer += deltaTime;

		// 一定間隔で敵を生成
		if (spawnTimer >= spawnInterval)
		{
			Spawn();

			spawnTimer = 0.0f;
		}
	}

	// ゲームプレイ時間を計測
	playTimer += deltaTime;

	// ENEMY_GROW_INTERVALごとに敵が強くなる。(ステータスがenemyGrowRate倍ずつ強化)
	enemyGrowRate = 1.0f + floor(playTimer / growInterval) * growRatePerInterval;

	// 敵の更新
	for (auto& e : enemies)
	{
		e->Update(player, deltaTime);
	}

	// 非アクティブな敵の削除
	enemies.erase(
		std::remove_if(enemies.begin(), enemies.end(),
			[](const std::unique_ptr<Enemy>& e) { return !e->IsActive();}),
		enemies.end()
	);

	// 敵の数が減った場合
	if (enemies.size() < enemyPrevCount)
	{
		// タイマーをリセットしてすぐに敵がスポーンしないようにする
		spawnTimer = 0.0f;
	}
}

void EnemyManager::Draw() const {

	for (const auto& e : enemies) {
		e->Draw();
	}
}

void EnemyManager::DrawUI() const {

	for (const auto& e : enemies)
	{
		e->DrawUI();
	}
}

// プレイヤーの攻撃が敵に当たったか判定
void EnemyManager::CheckPlayerAttackHit(const Player& player, ScoreManager& scoreManager, ItemManager& itemManager, SoundManager& soundManager) {

	PlayerAttackHitBox playerHitBox = player.GetPlayerAttackHitBox();

	// 攻撃判定が無効なら処理しない
	if (!playerHitBox.isAttackActive) return;

	for (auto& e : enemies)
	{
		// 非アクティブな敵は処理しない
		if (!e->IsActive()) continue;

		// プレイヤーの攻撃位置と敵の差分を求める
		float dx = e->GetPos().x - playerHitBox.x;
		float dz = e->GetPos().z - playerHitBox.z;

		// 距離の2乗を求める
		float distanceSq = dx * dx + dz * dz;

		// 円どうしの当たり判定計算のために半径の合計を求める
		float totalRadius = e->GetBodyRadius() + playerHitBox.radius;

		if (distanceSq < totalRadius * totalRadius)
		{
			// HPが0以下になった場合
			if (e->TakeDamage(player.GetAttackPower(), soundManager))
			{
				// スコア加算
				scoreManager.AddScore(e->GetEnemyType(), e->GetScoreValue());

				// 倒した敵の数を種類ごとに加算
				CountEnemyKill(e->GetEnemyType());

				// 回復アイテム生成依頼(確率)
				itemManager.DropRecoveryItem(e->GetPos().x, e->GetPos().y, e->GetPos().z);

				// 経験値アイテム生成依頼(確定)
				itemManager.DropExpItem(e->GetPos().x, e->GetPos().y, e->GetPos().z);
			}
		}
	}
}

// 敵の攻撃がプレイヤーに当たったか判定
void EnemyManager::CheckEnemyAttackHit(Player& player, SoundManager& soundManager) {

	for (auto& e : enemies)
	{
		// 非アクティブな敵は処理しない
		if (!e->IsActive()) continue;

		EnemyAttackHitBox enemyHitBox = e->GetHitBox();

		// 攻撃判定が無効なら処理しない
		if (!enemyHitBox.isAttackActive) continue;

		float dx = player.GetPosition().x - enemyHitBox.x;
		float dz = player.GetPosition().z - enemyHitBox.z;

		float distanceSq = dx * dx + dz * dz;

		float totalRadius = player.GetBodyRadius() + enemyHitBox.radius;

		if (distanceSq < totalRadius * totalRadius)
		{
			// 攻撃力を取得してその値分のダメージ
			player.TakeDamage(e->GetAttackPower(), soundManager);
		}
	}
}

void EnemyManager::CountEnemyKill(EnemyType type) {

	// 倒した敵の数を種類ごとに加算、保持
	killCount[type]++;
}