#include "MagicEffect.h"


#include "GameConfig.h"
#include "Player.h"


MagicEffect::MagicEffect(float startX, float startY, float startZ, const int* handles) {

	isActive = true;

	grHandles = handles;

	x = startX;
	y = startY;
	z = startZ;
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
				isLoop = false;
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

	if (x < GameConfig::FIELD_MIN_X || x > GameConfig::FIELD_MAX_X ||
		z < GameConfig::FIELD_MIN_Z || z > GameConfig::FIELD_MAX_Z)
	{
		isActive = false;
	}
}

void MagicEffect::Draw() const {

	if (!isActive) return;

	DrawBillboard3D(VGet(x, y, z), 0.5f, 0.5f, EFFECT_SCALE, 0.0f,
		grHandles[currentFrame], TRUE);
}
