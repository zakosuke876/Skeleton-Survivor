#pragma once
#include "Item.h"
#include <string>

// 前方宣言
class Player;
class SoundManager;

// 回復アイテムの種類
enum class RecoveryItemType {
	RECOVERY_SMALL,   // 小
	RECOVERY_MEDIUM,  // 中
	RECOVERY_LARGE,   // 大
	ITEM_TYPE_MAX,    // 配列サイズとして扱う
};

// JSONから読み込んだ回復アイテムのデータを格納する構造体
struct RecoveryItemData {
	int recoveryValue; // 回復力
	std::string graphPath; // 画像ファイルのパス
};


// enum classをintに変換する（配列のインデックスに使用するため）
constexpr int ToInt(RecoveryItemType type) {

	return static_cast<int>(type);
}

class RecoveryItem : public Item {
public:

	RecoveryItem(float posX, float posY, float posZ, int recoveryValue, int itemTexHandle);
	~RecoveryItem() override = default;
	void DrawModel() const override;
	void OnPickUp(Player& player, const SoundManager& soundManager) override;

private:

	// 回復力
	int recoveryPower = -1;
};