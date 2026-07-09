#include "RecoveryItem.h"
#include "Player.h"
#include "SoundManager.h"
#include "DxLib.h"

RecoveryItem::RecoveryItem(float posX, float posY, float posZ, int recoveryValue, int texHandle) {

	x = posX;
	y = posY;
	z = posZ;

	// 画像ハンドルを設定
	grHandle = texHandle;

	recoveryPower = recoveryValue;
}

void RecoveryItem::DrawModel() const {

	DrawBillboard3D(VGet(x, y, z), 0.5f, 0.5f, ITEM_SCALE, 0.0f, grHandle, TRUE);
}

void RecoveryItem::OnPickUp(Player& player, const SoundManager& soundManager) {

	// HPを回復させる
	player.Recovery(recoveryPower);

	// 効果音再生を依頼(引数でタイプを渡す)
	soundManager.PlaySe(RECOVERY);
}