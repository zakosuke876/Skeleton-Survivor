#pragma once
#include "MagicEffect.h"
#include <vector>

class Player;
class SoundManager;

class MagicEffectManager {
public:

	MagicEffectManager();
	~MagicEffectManager();

	void Reset();

	void Update(const Player& player, float deltaTime);
	void Draw() const;

	void CheckPlayerHit(Player& player, SoundManager& soundManager);

	void PlayMagic(float startX, float startY, float startZ);

private:

	std::vector<MagicEffect> effects;

	int grHandles[MagicEffect::EFFECT_ALL] = {};

	// –‚–@‚ÌÅ‘å“¯oŒ»”
	static constexpr int MAX_EFFECT = 5;
};