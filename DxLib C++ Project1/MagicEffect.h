#pragma once
#include "DxLib.h"

class Player;

class MagicEffect {
public:

	MagicEffect(float startX, float startY, float startZ, const int* handles, int damage);

	void Update(const Player& player, float deltaTime);

	void Draw() const;

	// オブジェクトが有効かどうかのフラグを返す
	bool IsActive() const { return isActive; }

	// 座標を取得する
	VECTOR GetPos() const { return VGet(x, y, z); }

	// 半径を取得する(当たり判定用)
	float GetRadius() const { return MAGIC_RADIUS; }

	// プレイヤーに与えるダメージを取得する
	int GetDamage() const { return damage; }

	// プレイヤーと触れた際にフラグを書き換え無効化する
	void SetActive(bool active) { isActive = active; }

	// ----------------------------------------
	// スプライトシート設定
	// ----------------------------------------

	// 分割総数
	static constexpr int EFFECT_ALL = 11;

	// 横の分割数
	static constexpr int EFFECT_DIV_X = 11;

	// 縦の分割数
	static constexpr int EFFECT_DIV_Y = 1;

	// 1コマの横幅
	static constexpr int EFFECT_WIDTH = 48;

	// 1コマの縦幅
	static constexpr int EFFECT_HEIGHT = 48;

private:

	// ----------------------------------------
	// 座標
	// ----------------------------------------

	float x = 0.0f;
	float y = 20.0f;
	float z = 0.0f;

	// ----------------------------------------
	// 当たり判定
	// ----------------------------------------

	// 当たり判定の半径
	float MAGIC_RADIUS = 20.0f;

	// ----------------------------------------
	// 移動
	// ----------------------------------------

	// 魔法の移動速度
	float speed = 0.4f;

	// ----------------------------------------
	// 攻撃力
	// ----------------------------------------

	// 魔法が与えるダメージ
	int damage = 0;

	// ----------------------------------------
	// フラグ
	// ----------------------------------------

	bool isActive = false;

	// ----------------------------------------
	// アニメーション
	// ----------------------------------------

	// 画像ハンドル配列
	const int* grHandles = nullptr;

	// アニメーションの開始・終了コマ
	static constexpr int BOLT_START = 0;
	static constexpr int BOLT_END   = 3;

	// 着弾アニメーションの開始・終了コマ
	static constexpr int IMPACT_START = 5;
	static constexpr int IMPACT_END = 10;

	// 現在再生中の開始・終了コマ
	int startFrame = BOLT_START;
	int endFrame = BOLT_END;

	// ループ再生するか
	bool isLoop = true;

	// 現在のコマ番号
	int currentFrame = BOLT_START;

	// コマ送り用タイマー
	float frameTimer = 0.0f;

	// 1コマの表示時間(秒)
	static constexpr float FRAME_INTERVAL = 1.0f / 15.0f;

	// 描画時の拡大倍率
	static constexpr float EFFECT_SCALE = 50.0f;

	// ----------------------------------------
	// 生存時間
	// ----------------------------------------

	// 生存時間の計測
	float lifeTimer = -1.0f;

	// 生存時間(秒)
	static constexpr float LIFE_TIME = 3.0f;
};