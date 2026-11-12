#include "ItemManager.h"
#include "SoundManager.h"
#include "ExpItem.h"
#include "Player.h"
#include "ItemDropConfigLoader.h"
#include "JsonPath.h"
#include "DxLib.h"
#include "RecoveryItemDataLoader.h"
#include "ExpItemDataLoader.h"
#include "Random.h"
#include "CollisionSystem.h"

ItemManager::ItemManager(Random& random, const CollisionSystem& collisionSystem)
	: random(random), collisionSystem(collisionSystem)
{

	// JSONファイルを読み込み、アイテムドロップ関係のデータを取得する
	ItemDropConfig dropConfig;
	if (!ItemDropConfigLoader::Load(dropConfig))
	{
		printfDx("アイテムドロップデータの読み込みに失敗しました\n");
	}

	dropRate = dropConfig.dropRate;

	dropOffsetY = dropConfig.dropOffsetY;

	expItemRandomOffset = dropConfig.expItemRandomOffset;

	itemTypeRate = dropConfig.itemTypeRate;

	if (!RecoveryItemDataLoader::Load(recoveryItemDataTable))
	{
		printfDx("回復アイテムデータの読み込みに失敗しました\n");
	}

	// 経験値アイテムデータを読み込む
	if (!ExpItemDataLoader::Load(expItemDataTable))
	{
		printfDx("経験値アイテムデータの読み込みに失敗しました\n");
	}

	// JSONから取得したgraphPathを使ってテクスチャを読み込む
	for (int i = 0; i < ToInt(RecoveryItemType::ITEM_TYPE_MAX); i++)
	{
		RecoveryTexHandles[i] = LoadGraph(recoveryItemDataTable[i].graphPath.c_str());
	}

	// テクスチャ読み込み
	expTexHandle = LoadGraph(expItemDataTable.graphPath.c_str());
}

ItemManager::~ItemManager() {

	// リソース開放
	for (int i = 0; i < ToInt(RecoveryItemType::ITEM_TYPE_MAX); i++)
	{
		DeleteGraph(RecoveryTexHandles[i]);
	}

	DeleteGraph(expTexHandle);
}

void ItemManager::Update() {

	// 非アクティブなアイテムの削除
	items.erase(
		std::remove_if(items.begin(), items.end(),
			[](const std::unique_ptr<Item>& item) { return !item->IsActive();}),
		items.end()
	);
}

void ItemManager::Reset() {

	items.clear();
}

void ItemManager::ItemDrop(float posX, float posY, float posZ, RecoveryItemType itemType) {

	int recoveryValue = recoveryItemDataTable[ToInt(itemType)].recoveryValue;

	items.emplace_back(std::make_unique<RecoveryItem>(posX, posY + dropOffsetY, posZ, recoveryValue, RecoveryTexHandles[ToInt(itemType)]));
}

bool ItemManager::ItemDropCheck() {

	// アイテムドロップの抽選(1～100の範囲で乱数生成)
	int dropChance = random.GetRandomInt(ITEM_DROP_CHANCE_MIN, ITEM_DROP_CHANCE_MAX);

	// ドロップ条件を満たした場合
	return dropChance <= dropRate;
}

void ItemManager::DropRecoveryItem(float posX, float posY, float posZ) {

	// ドロップしなかった場合は処理を終了
	if (!ItemDropCheck())
	{
		return;
	}

	// アイテムのタイプを抽選
	RecoveryItemType itemType = GetDropItemType();

	// アイテム生成
	ItemDrop(posX, posY, posZ, itemType);
}

void ItemManager::DropExpItem(float posX, float posY, float posZ) {

	int expValue = expItemDataTable.expValue;

	float randPosX = random.GetRandomFloat(-expItemRandomOffset, expItemRandomOffset);
	float randPosZ = random.GetRandomFloat(-expItemRandomOffset, expItemRandomOffset);

	// 経験値アイテムを生成(X,Z座標をランダムにずらす)
	items.emplace_back(std::make_unique<ExpItem>(posX + randPosX, posY + dropOffsetY, posZ + randPosZ, expValue, expTexHandle));
}


void ItemManager::Draw() const {

	for (const auto& item : items)
	{
		item->Draw();
	}
}

void ItemManager::CheckPlayerCollision(Player& player, const SoundManager& soundManager) {

	for (auto& item : items)
	{
		if (collisionSystem.IsCircleHit(player.GetPosition().x, player.GetPosition().z, player.GetBodyRadius(),
			item->GetPosition().x, item->GetPosition().z, item->GetRadius()))
		{
			item->OnPickUp(player, soundManager);
			item->SetActive(false);
		}
	}
}

RecoveryItemType ItemManager::GetDropItemType() {

	// 指定した範囲内の乱数を生成
	int typeChance = random.GetRandomInt(ITEM_TYPE_RANGE_MIN, ITEM_TYPE_RANGE_MAX);

	// 累積値を計算(smallは smallまで, mediumは small + mediumまで)
	int smallMax  = itemTypeRate.smallRate;
	int mediumMax = smallMax + itemTypeRate.mediumRate;

	if (typeChance <= smallMax) return RecoveryItemType::RECOVERY_SMALL;
	if (typeChance <= mediumMax) return RecoveryItemType::RECOVERY_MEDIUM;
	return RecoveryItemType::RECOVERY_LARGE;
}
