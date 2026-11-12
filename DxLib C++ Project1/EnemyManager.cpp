#include "EnemyManager.h"
#include "ScoreManager.h"
#include "ItemManager.h"
#include "SoundManager.h"
#include "Player.h"
#include "Skeleton_Normal.h"
#include "Skeleton_Warrior.h"
#include "Skeleton_Mage.h"
#include "EnemyNormalDataLoader.h"
#include "EnemyWarriorDataLoader.h"
#include "EnemyMageDataLoader.h"
#include "SpawnDataLoader.h"
#include "GameConfig.h"
#include "Random.h"
#include "CollisionSystem.h"
#include <algorithm>
#include <cmath>
#include "DxLib.h"


#include "MagicEffectManager.h"

EnemyManager::EnemyManager(Random& random, const CollisionSystem& collisionSystem)
	: random(random), collisionSystem(collisionSystem)
{
	// JSONからSkeleton_Normalのデータ読み込みを指示
	if (!EnemyNormalDataLoader::Load(normalStatus))
	{
		printfDx("ノーマルスケルトンのデータ読み込みに失敗しました\n");
	}

	// JSONからSkeleton_Warriorのデータ読み込みを指示
	if (!EnemyWarriorDataLoader::Load(warriorStatus))
	{
		printfDx("ウォーリアースケルトンのデータ読み込みに失敗しました\n");
	}

	// JSONからSkeleton_Mageのデータ読み込みを指示
	if (!EnemyMageDataLoader::Load(mageStatus))
	{
		printfDx("メイジスケルトンのデータ読み込みに失敗しました\n");
	}

	// JSONから敵のスポーン情報のデータ読み込みを指示
	if (!SpawnDataLoader::Load(spawnConfig))
	{
		printfDx("スポーンデータの読み込みに失敗しました\n");
	}

	// モデルを読み込む
	skeletonNormalModelHandle  = MV1LoadModel(normalStatus.enemyStatus.modelPath.c_str());
	skeletonWarriorModelHandle = MV1LoadModel(warriorStatus.enemyStatus.modelPath.c_str());
	skeletonMageModelHandle = MV1LoadModel(mageStatus.enemyStatus.modelPath.c_str());

	// テクスチャを読み込む
	skeletonNormalTexHandle = LoadGraph(normalStatus.enemyStatus.texturePath.c_str());
	skeletonWarriorTexHandle = LoadGraph(warriorStatus.enemyStatus.texturePath.c_str());
	skeletonMageTexHandle = LoadGraph(mageStatus.enemyStatus.texturePath.c_str());

	// スポーン関連のデータを読み込む
	spawnInterval = spawnConfig.spawnInterval;

	maxEnemy = spawnConfig.maxEnemy;

	growInterval = spawnConfig.growInterval;

	growRatePerInterval = spawnConfig.growRatePerInterval;

	// 武器の読み込み
	staffModelHandle = MV1LoadModel("Model/Staff/Mage_Staff.mv1");
	staffTexHandle   = LoadGraph("Model/Staff/skeleton_texture.png");
}

EnemyManager::~EnemyManager() {

	// リソース開放
	MV1DeleteModel(skeletonNormalModelHandle);
	MV1DeleteModel(skeletonWarriorModelHandle);
	MV1DeleteModel(skeletonMageModelHandle);
	MV1DeleteModel(staffModelHandle);

	DeleteGraph(skeletonNormalTexHandle);
	DeleteGraph(skeletonWarriorTexHandle);
	DeleteGraph(skeletonMageTexHandle);
	DeleteGraph(staffTexHandle);
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

	// スポーン X/Z 座標を乱数で設定
	float x = random.GetRandomFloat(GameConfig::FIELD_MIN_X, GameConfig::FIELD_MAX_X);
	float z = random.GetRandomFloat(GameConfig::FIELD_MIN_Z, GameConfig::FIELD_MAX_Z);

	// 生成する敵の種類を乱数で決定
	int enemyTypeNum = random.GetRandomInt(ENEMY_NORMAL, ENEMY_TYPE_COUNT - 1);

	// 値によって生成する敵の種類を変える
	switch (enemyTypeNum)
	{
		case ENEMY_NORMAL:

			// 敵を生成
			enemies.emplace_back(std::make_unique<Skeleton_Normal>(x, 0.0f, z, skeletonNormalModelHandle, skeletonNormalTexHandle, normalStatus, enemyGrowRate, random));

			break;

		case ENEMY_WARRIOR:

			enemies.emplace_back(std::make_unique<Skeleton_Warrior>(x, 0.0f, z, skeletonWarriorModelHandle, skeletonWarriorTexHandle, warriorStatus, enemyGrowRate, random));

			break;

		case ENEMY_MAGE:

			enemies.emplace_back(std::make_unique<Skeleton_Mage>(x, 0.0f, z, skeletonMageModelHandle, skeletonMageTexHandle, mageStatus, staffModelHandle, staffTexHandle, enemyGrowRate, random));

			break;
	}
}

void EnemyManager::Update(Player& player, float deltaTime, MagicEffectManager& magicEffectManger) {

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

	// growIntervalごとに敵が強くなる。(ステータスがenemyGrowRate倍ずつ強化)
	enemyGrowRate = 1.0f + floor(playTimer / growInterval) * growRatePerInterval;

	// 敵の更新
	for (auto& e : enemies)
	{
		e->Update(player, deltaTime, magicEffectManger);
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
void EnemyManager::CheckPlayerAttackHit(const Player& player, ScoreManager& scoreManager, ItemManager& itemManager, const SoundManager& soundManager) {

	PlayerAttackHitBox playerHitBox = player.GetPlayerAttackHitBox();

	// 攻撃判定が無効なら処理しない
	if (!playerHitBox.isAttackActive) return;

	for (auto& e : enemies)
	{
		// 非アクティブな敵は処理しない
		if (!e->IsActive()) continue;

		if (collisionSystem.IsCircleHit(playerHitBox.x, playerHitBox.z, playerHitBox.radius,
			e->GetPos().x, e->GetPos().z, e->GetBodyRadius()))
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
void EnemyManager::CheckEnemyAttackHit(Player& player, const SoundManager& soundManager) {

	for (auto& e : enemies)
	{
		// 非アクティブな敵は処理しない
		if (!e->IsActive()) continue;

		EnemyAttackHitBox enemyHitBox = e->GetHitBox();

		// 攻撃判定が無効なら処理しない
		if (!enemyHitBox.isAttackActive) continue;

		if (collisionSystem.IsCircleHit(enemyHitBox.x, enemyHitBox.z, enemyHitBox.radius,
			player.GetPosition().x, player.GetPosition().z, player.GetBodyRadius()))
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
