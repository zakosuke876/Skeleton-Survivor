#pragma once
#include "DxLib.h"
#include <array>
#include "AnimationClip.h"
#include "Effect.h"

// 前方宣言
class SpriteManager;

class EffectManager {
public:

	EffectManager(const SpriteManager& spriteManager);
	void Spawn(VECTOR pos, EffectType type, float delayTime = 0.0f);
	void Update(float deltaTime);
	void Draw() const;

private:

	// Gameが所有する本体を参照で借用
	const SpriteManager& spriteManager;

	// 同時再生できるエフェクトの最大数
	static constexpr int MAX_EFFECT = 20;

	std::array<Effect, MAX_EFFECT> effects{};
};
