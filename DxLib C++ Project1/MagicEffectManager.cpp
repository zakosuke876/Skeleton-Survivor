#include "MagicEffectManager.h"
#include "Player.h"
#include "CollisionSystem.h"
#include "DxLib.h"
#include <algorithm>

MagicEffectManager::MagicEffectManager(const CollisionSystem& collisionSystem)
	: collisionSystem(collisionSystem)
{

	// スプライトシートを分割して読み込む
	LoadDivGraph("Textures/Effect/Magic/Firebolt SpriteSheet.png", MagicEffect::EFFECT_ALL, MagicEffect::EFFECT_DIV_X, MagicEffect::EFFECT_DIV_Y, MagicEffect::EFFECT_WIDTH, MagicEffect::EFFECT_HEIGHT, grHandles);
}

MagicEffectManager::~MagicEffectManager() {

	// リソース開放
	for (int i = 0; i < MagicEffect::EFFECT_ALL; i++)
	{
		DeleteGraph(grHandles[i]);
	}

	effects.clear();
}

void MagicEffectManager::Reset() {

	effects.clear();
}

void MagicEffectManager::PlayMagic(float startX, float startY, float startZ, int damage) {

	// 上限に達している場合は生成しない
	if (effects.size() >= MAX_EFFECT) return;

	// 指定座標に魔法を生成
	effects.emplace_back(startX, startY, startZ, grHandles, damage);
}

void MagicEffectManager::CheckPlayerHit(Player& player, const SoundManager& soundManager) {

	for (auto& e : effects)
	{
		// 非アクティブな魔法は処理しない
		if (!e.IsActive()) continue;

		if (collisionSystem.IsCircleHit(e.GetPosition().x, e.GetPosition().z, e.GetRadius(),
			player.GetPosition().x, player.GetPosition().z, player.GetBodyRadius()))
		{
			// 当たった魔法を無効化
			e.SetActive(false);

			// プレイヤーにダメージを与える
			player.TakeDamage(e.GetDamage(), soundManager);
		}
	}
}

void MagicEffectManager::Update(const Player& player, float deltaTime) {

	// 魔法の更新
	for (auto& e : effects)
	{
		e.Update(player, deltaTime);
	}

	// 非アクティブな魔法の削除
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
}
