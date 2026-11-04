#include "GroundEffect.h"
#include "DxLib.h"

GroundEffect::GroundEffect() {

	std::random_device rd;
	rng = std::mt19937(rd());
}

void GroundEffect::Update(float deltaTime) {

	if (spawnTimer > 0)
	{
		spawnTimer -= deltaTime;

		if (spawnTimer <= 0)
		{
			spawnTimer = 0.0f;
		}
	}

	for (Particle& p : particles)
	{
		if (!p.isActive) continue;

		// 寿命を計測
		p.timer += deltaTime;

		p.vY -= GRAVITY * deltaTime;

		// 位置更新
		p.x += p.vX * deltaTime;
		p.y += p.vY * deltaTime;
		p.z += p.vZ * deltaTime;

		// 寿命が来たら無効化
		if (p.timer >= p.lifeTime) p.isActive = false;
	}
}

void GroundEffect::Spawn(float startX, float startZ, unsigned int effectColor) {

	if (spawnTimer > 0) return;

	for (Particle& p : particles)
	{
		if (p.isActive) continue;

		p.x = startX;
		p.y = 0.0f;
		p.z = startZ;

		std::uniform_real_distribution<float> randVector(MIN_RAND_VECTOR, MAX_RAND_VECTOR);
		std::uniform_real_distribution<float> randYVector(MIN_RAND_Y_VECTOR, MAX_RAND_Y_VECTOR);

		// 飛散速度を乱数で生成して設定
		p.vX = randVector(rng);
		p.vY = randYVector(rng);
		p.vZ = randVector(rng);

		p.lifeTime = PARTICLE_LIFETIME;
		p.timer    = 0.0f;
		p.isActive = true;

		p.color = effectColor;

		spawnTimer = PARTICLE_INTERVAL;

		break;
	}
}

void GroundEffect::Draw() const {

	for (const Particle& p : particles)
	{
		if (!p.isActive) continue;

		// エフェクトを球で描画
		DrawSphere3D(VGet(p.x, p.y, p.z), PARTICLE_RADIUS, PARTICLE_DIV_NUM, p.color, p.color, TRUE);
	}
}
