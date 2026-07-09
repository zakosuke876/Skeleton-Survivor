#pragma once
#include "RecoveryItem.h"
#include "ExpItem.h"
#include "ItemDropConfig.h"
#include <random>
#include <vector>
#include <array>

// 前方宣言
class Player;
class SoundManager;

class ItemManager {
public:

	ItemManager();
	~ItemManager();
	void Reset(); // ゲームリセット処理
	void Update();
	void Draw() const;
	void ItemDrop(float posX, float posY, float posZ, RecoveryItemType itemType);
	bool ItemDropCheck();
	void DropRecoveryItem(float posX, float posY, float posZ);
	void DropExpItem(float posX, float posY, float posZ);
	void CheckPlayerCollision(Player& player, SoundManager& soundManager);
	RecoveryItemType GetDropItemType();

private:

	// アイテム管理
	std::vector<std::unique_ptr<Item>> items;

	// JSONから読み込んだ回復アイテムのデータ保持
	std::array<RecoveryItemData, ToInt(RecoveryItemType::ITEM_TYPE_MAX)> recoveryItemDataTable;

	// JSONから読み込んだ経験値アイテムのデータ保持
	ExpItemData expItemDataTable;

	// テクスチャハンドルを管理
	int RecoveryTexHandles[ToInt(RecoveryItemType::ITEM_TYPE_MAX)] = {};

	// 経験値アイテムテクスチャハンドル
	int expTexHandle = -1;

	// アイテムドロップの判定に使う乱数範囲
	static constexpr int ITEM_DROP_CHANCE_MIN = 1;
	static constexpr int ITEM_DROP_CHANCE_MAX = 100;

	// JSONから読み込む値
	// アイテムドロップ率(%)
	int dropRate = -1;

	// アイテムの生成座標をずらすために使う乱数範囲
	float dropOffsetY = -1.0f;

	// 経験値アイテムの生成座標をずらす範囲
	float expItemRandomOffset = -1.0f;

	// アイテム種類ごとのドロップ確率(JSONから読み込む)
	ItemTypeRate itemTypeRate;

	// アイテムの種類判定に使う乱数範囲
	static constexpr int ITEM_TYPE_RANGE_MIN = 0;
	static constexpr int ITEM_TYPE_RANGE_MAX = 100;

	// ランダムエンジン
	std::mt19937 rng;
};