#include "MagicEffectManager.h"
#include "Player.h"
#include "DxLib.h"
#include <algorithm>

MagicEffectManager::MagicEffectManager() {

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

void MagicEffectManager::PlayMagic(float startX, float startY, float startZ) {

	// 上限に達している場合は生成しない
	if (effects.size() >= MAX_EFFECT) return;

	// 指定座標に魔法を生成
	effects.emplace_back(startX, startY, startZ, grHandles);
}

void MagicEffectManager::CheckPlayerHit(Player& player, SoundManager& soundManager) {

	for (auto& e : effects)
	{
		// 非アクティブな魔法は処理しない
		if (!e.IsActive()) continue;

		// 魔法とプレイヤーの差分を求める
		float dx = player.GetPosition().x - e.GetPos().x;
		float dz = player.GetPosition().z - e.GetPos().z;

		// 距離の2乗を求める
		float distanceSq = dx * dx + dz * dz;

		// 円どうしの当たり判定計算のために半径の合計を求める
		float totalRadius = player.GetBodyRadius() + e.GetRadius();

		if (distanceSq < totalRadius * totalRadius)
		{
			// 当たった魔法を無効化
			e.SetActive(false);

			// プレイヤーにダメージを与える
			player.TakeDamage(20, soundManager);
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