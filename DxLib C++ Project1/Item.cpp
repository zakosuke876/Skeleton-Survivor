#include "Item.h"
#include "DxLib.h"

Item::Item() {

	// —LŒø‰»
	isActive = true;
}

void Item::Draw() const {

	// –³Œø‰»ó‘Ô‚È‚çˆ—‚µ‚È‚¢
	if (!isActive) return;

	DrawModel();
}