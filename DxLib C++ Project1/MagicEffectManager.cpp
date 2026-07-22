#include "MagicEffectManager.h"
#include "Player.h"
#include "DxLib.h"
#include <algorithm>

MagicEffectManager::MagicEffectManager() {
	LoadDivGraph("Textures/Effect/Magic/Firebolt SpriteSheet.png", MagicEffect::EFFECT_ALL, MagicEffect::EFFECT_DIV_X, MagicEffect::EFFECT_DIV_Y, MagicEffect::EFFECT_WIDTH, MagicEffect::EFFECT_HEIGHT, grHandles);

	magicTimer = 0.0f;
}

MagicEffectManager::~MagicEffectManager() {

	for (int i = 0; i < MagicEffect::EFFECT_ALL; i++)
	{
		DeleteGraph(grHandles[i]);
	}

	effects.clear();
}

void MagicEffectManager::PlayMagic(float startX, float startY, float startZ) {

	effects.emplace_back(startX, startY, startZ, grHandles);
}

void MagicEffectManager::CheckPlayerHit(Player& player, SoundManager& soundManager) {

	for (auto& e : effects)
	{
		if (!e.IsActive()) continue;

		float dx = player.GetPosition().x - e.GetPos().x;
		float dz = player.GetPosition().z - e.GetPos().z;

		float distanceSq = dx * dx + dz * dz;

		float totalRadius = player.GetBodyRadius() + e.GetRadius();

		if (distanceSq < totalRadius * totalRadius)
		{
			e.SetActive(false);
			player.TakeDamage(20, soundManager);
		}
	}
}

void MagicEffectManager::Update(const Player& player, float deltaTime) {

	for (auto& e : effects)
	{
		e.Update(player, deltaTime);
	}

	if (effects.size() < MAX_EFFECT)
	{
		magicTimer += deltaTime;
	}

	if (effects.size() < MAX_EFFECT && magicTimer >= MAGIC_INTERVAL)
	{
		PlayMagic(0.0f, 20.0f, 100.0f);
		magicTimer = 0.0f;
	}

	effects.erase(
		std::remove_if(effects.begin(), effects.end(),
			[](const MagicEffect& e) { return !e.IsActive(); }),
		effects.end()
	);
}

void MagicEffectManager::Draw() const {

	for (const auto& e : effects)
	{
		e.Draw();
	}

	DrawFormatString(200, 200, GetColor(255, 0, 0), "êî : %d", effects.size());
}