#pragma once
#include "DxLib.h"

// 前方宣言
class Player;
class SoundManager;

class Item {
public:

	virtual ~Item() = default;
	Item();
	void Draw() const;

	// 座標を取得する
	VECTOR GetPosition() const { return VGet(x, y, z); }

	// 半径を取得する(当たり判定用)
	float GetRadius() const { return ITEM_RADIUS; }

	// オブジェクトが有効かどうかのフラグを返す
	bool IsActive() const { return isActive; }

	// プレイヤーと触れた際にフラグを書き換え無効化する
	void SetActive(bool active) { isActive = active; }

	// アイテム固有の処理をさせる
	virtual void OnPickUp(Player& player, const SoundManager& soundManager) = 0;

protected:

	virtual void DrawModel() const = 0;

	// 画像ハンドル
	int grHandle = -1;

	// 座標
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	// 半径
	static constexpr float ITEM_RADIUS = 10.0f;

	// モデルの拡大倍率
	static constexpr float ITEM_SCALE = 20.0f;

	// オブジェクトが有効かどうか
	bool isActive = false;
};