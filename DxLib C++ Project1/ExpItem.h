#pragma once
#include "Item.h"
#include <string>

// 前方宣言
class Player;
class SoundManager;

struct ExpItemData {
	int expValue; // 得られる経験値量
	std::string graphPath; // 画像ファイルのパス
};

class ExpItem : public Item {
public:

	ExpItem(float posX, float posY, float posZ, int expValue, int texHandle);
	~ExpItem() override = default;
	void DrawModel() const override;
	void OnPickUp(Player& player, const SoundManager& soundManager) override;

private:

	// 得られる経験値
	int addExpValue = -1;
};