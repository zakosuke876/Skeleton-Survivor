#include "MagicEffect.h"
#include "DxLib.h"


#include "Player.h"


MagicEffect::MagicEffect() {

	LoadDivGraph("Textures/Effect/Magic/Firebolt SpriteSheet.png", EFFECT_ALL, EFFECT_DIV_X, EFFECT_DIV_Y, EFFECT_WIDTH, EFFECT_HEIGHT, grHandles);
}

MagicEffect::~MagicEffect() {

	for (int i = 0; i < EFFECT_ALL; i++)
	{
		DeleteGraph(grHandles[i]);
	}
}

void MagicEffect::Update(const Player& player, float deltaTime) {

	frameTimer += deltaTime;

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

			}
		}
	}

	float dx = player.GetPosition().x - x;
	float dz = player.GetPosition().z - z;

	float distance = sqrt(dx * dx + dz * dz);

	if (distance > 0.0f)
	{
		float dirX = dx / distance;
		float dirZ = dz / distance;

		x += dirX * speed;
		z += dirZ * speed;
	}
}

void MagicEffect::Draw() const {

	DrawBillboard3D(VGet(x, y, z), 0.5f, 0.5f, EFFECT_SCALE, 0.0f,
		grHandles[currentFrame], TRUE);

	DrawFormatString(100, 500, GetColor(255, 0, 0), "座標 : X %f Z %f", x, z);


	// 当たり判定確認
	/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawSphere3D(VGet(x, y, z), 8.0f, 16,
		GetColor(255, 0, 0), GetColor(255, 255, 255), FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);*/
}
