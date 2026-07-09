#include "ExpItem.h"
#include "Player.h"
#include "SoundManager.h"
#include "DxLib.h"

ExpItem::ExpItem(float posX, float posY, float posZ, int expValue, int texHandle) {

	x = posX;
	y = posY;
	z = posZ;

	// 得られる経験値を設定
	addExpValue = expValue;

	// 画像ハンドルを設定
	grHandle = texHandle;
}

void ExpItem::DrawModel() const {

	DrawBillboard3D(VGet(x, y, z), 0.5f, 0.5f, ITEM_SCALE, 0.0f, grHandle, TRUE);
}

void ExpItem::OnPickUp(Player& player, const SoundManager& soundManager) {

	// 経験値を加算する
	player.AddExp(addExpValue, soundManager);

	// 効果音再生を依頼(引数でタイプを渡す)
	soundManager.PlaySe(EXP);
}