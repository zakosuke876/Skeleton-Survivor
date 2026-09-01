#include "Item.h"
#include "DxLib.h"

Item::Item() {

	// 有効化
	isActive = true;
}

void Item::Draw() const {

	// 無効化状態なら処理しない
	if (!isActive) return;

	DrawModel();
}