#include <cstring>
#include "DxLib.h"
#include "InputManager.h"

void InputManager::Update() {

	// 前フレームの状態をコピー
	memcpy(prevKey, currentKey, sizeof(currentKey));

	// 今のフレームの状態で上書き
	GetHitKeyStateAll(currentKey);
}

bool InputManager::IsDown(int keyCode) const {

	// キー入力範囲外の場合処理しない
	if (keyCode >= KEY_COUNT || keyCode < 0) return false;

	// 特定のキーが押されている場合
	return currentKey[keyCode] != 0;
}

bool InputManager::IsPressed(int keyCode) const {

	if (keyCode >= KEY_COUNT || keyCode < 0) return false;

	// 今押されていて、かつ前フレーム押されていない場合
	return prevKey[keyCode] == 0 && currentKey[keyCode] != 0;
}
