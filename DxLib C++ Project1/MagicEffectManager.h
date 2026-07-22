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

	// 魔法の最大同時出現数
	static constexpr int MAX_EFFECT = 5;

	// 生成タイマー
	float magicTimer = -1.0f;

	// 生成間隔(秒)
	static constexpr float MAGIC_INTERVAL = 2.0f;
};