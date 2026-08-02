#pragma once
#include "MagicEffect.h"
#include <vector>

// 前方宣言
class Player;
class SoundManager;

class MagicEffectManager {
public:

	MagicEffectManager();
	~MagicEffectManager();

	void Reset(); // ゲームリセット処理

	void Update(const Player& player, float deltaTime);
	void Draw() const;

	// 魔法とプレイヤーの当たり判定
	void CheckPlayerHit(Player& player, SoundManager& soundManager);

	// 指定座標に魔法を生成する
	void PlayMagic(float startX, float startY, float startZ, int damage);

private:

	// 魔法エフェクト管理
	std::vector<MagicEffect> effects;

	// 分割読み込みした画像ハンドルを保持する配列
	int grHandles[MagicEffect::EFFECT_ALL] = {};

	// 魔法の最大同時出現数
	static constexpr int MAX_EFFECT = 10;
};