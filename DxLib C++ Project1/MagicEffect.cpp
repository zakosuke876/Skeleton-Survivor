#include "MagicEffect.h"
#include "DxLib.h"

MagicEffect::MagicEffect() {

	LoadDivGraph("Textures/Effect/Magic/Firebolt SpriteSheet.png", EFFECT_ALL, EFFECT_DIV_X, EFFECT_DIV_Y, EFFECT_WIDTH, EFFECT_HEIGHT, grHandles);
}

MagicEffect::~MagicEffect() {

	for (int i = 0; i < EFFECT_ALL; i++)
	{
		DeleteGraph(grHandles[i]);
	}
}

void MagicEffect::Update(float deltaTime) {

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
}

void MagicEffect::Draw() const {

	DrawBillboard3D(VGet(x, y, z), 0.5f, 0.5f, EFFECT_SCALE, 0.0f,
		grHandles[currentFrame], TRUE);


	// 当たり判定確認
	/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawSphere3D(VGet(x, y, z), 8.0f, 16,
		GetColor(255, 0, 0), GetColor(255, 255, 255), FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);*/
}
