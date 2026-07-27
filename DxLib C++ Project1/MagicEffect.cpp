#include "MagicEffect.h"
#include "GameConfig.h"
#include "Player.h"


MagicEffect::MagicEffect(float startX, float startY, float startZ, const int* handles) {

	isActive = true;

	lifeTimer = 0.0f;

	// 画像ハンドル配列を設定
	grHandles = handles;

	x = startX;
	y = startY;
	z = startZ;
}

void MagicEffect::Update(const Player& player, float deltaTime) {

	// コマ送りタイマーを進める
	frameTimer += deltaTime;

	// アニメーションを再生
	if (frameTimer >= FRAME_INTERVAL)
	{
		frameTimer = 0.0f;
		currentFrame++;

		// 最終コマまで再生した場合
		if (currentFrame > endFrame)
		{
			// ループするか
			if (isLoop)
			{
				currentFrame = startFrame;
			}
			else
			{
				// 最終コマで止める
				currentFrame = endFrame;
			}
		}
	}
	
	// 生存時間を計測
	lifeTimer += deltaTime;

	// 生存時間を超えた場合
	if (lifeTimer >= LIFE_TIME)
	{
		isActive = false;
	}

	// 魔法はプレイヤーを追従させる
	float dx = player.GetPosition().x - x;
	float dz = player.GetPosition().z - z;

	// 距離を求める
	float distance = sqrt(dx * dx + dz * dz);

	// 0除算対策
	if (distance > 0.0f)
	{
		// 正規化
		float dirX = dx / distance;
		float dirZ = dz / distance;

		// プレイヤー方向へ移動
		x += dirX * speed;
		z += dirZ * speed;
	}

	// フィールド外に出た場合消える
	if (x < GameConfig::FIELD_MIN_X || x > GameConfig::FIELD_MAX_X ||
		z < GameConfig::FIELD_MIN_Z || z > GameConfig::FIELD_MAX_Z)
	{
		isActive = false;
	}
}

void MagicEffect::Draw() const {

	// 無効化状態なら処理しない
	if (!isActive) return;

	// ビルボードで魔法エフェクトを描画
	DrawBillboard3D(VGet(x, y, z), 0.5f, 0.5f, EFFECT_SCALE, 0.0f,
		grHandles[currentFrame], TRUE);
}
