#include "EffectManager.h"
#include "SpriteManager.h"

EffectManager::EffectManager(const SpriteManager& spriteManager)
	: spriteManager(spriteManager)
{

}

void EffectManager::Spawn(VECTOR pos, EffectType type, float delayTime) {

	AnimationClip clip = spriteManager.GetClip(type);

	for (auto& e : effects)
	{
		if (e.IsActive()) continue;

		// 生成座標とエフェクトの再生データを渡す
		e.Play(pos, clip, delayTime);

		break;
	}
}

void EffectManager::Update(float deltaTime) {

	for (auto& e : effects)
	{
		if (!e.IsActive()) continue;

		e.Update(deltaTime);
	}
}

void EffectManager::Draw() const {

	for (auto& e : effects)
	{
		if (!e.IsActive()) continue;

		e.Draw();
	}
}

void EffectManager::Reset() {

	// 全スロットをデフォルトに戻す
	effects.fill(Effect{});
}
