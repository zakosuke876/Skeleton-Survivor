#pragma once
#include "MagicEffect.h"
#include <vector>

// 前方宣言
class Player;
class SoundManager;
class CollisionSystem;
class EffectManager;

class MagicEffectManager {
public:

	MagicEffectManager(const CollisionSystem& collisionSystem, EffectManager& effectManager);
	~MagicEffectManager();

	void Reset(); // ゲームリセット処理

	void Update(const Player& player, float deltaTime);
	void Draw() const;

	// 魔法とプレイヤーの当たり判定
	void CheckPlayerHit(Player& player, const SoundManager& soundManager);

	// 指定座標に魔法を生成する
	void PlayMagic(float startX, float startY, float startZ, int damage);

private:

	// Gameが所有する本体を参照で借用
	const CollisionSystem& collisionSystem;
	EffectManager& effectManager;

	// 魔法エフェクト管理
	std::vector<MagicEffect> effects;

	// 右向き/左向き魔法エフェクトのパス
	static constexpr const char* MAGIC_LEFT_PATH = "Textures/Effect/Magic/FireboltSpriteSheetLeft.png";
	static constexpr const char* MAGIC_RIGHT_PATH = "Textures/Effect/Magic/FireboltSpriteSheetRight.png";

	// 分割読み込みした画像ハンドル(右向き・左向き)
	int grHandlesRight[MagicEffect::EFFECT_ALL] = {};
	int grHandlesLeft [MagicEffect::EFFECT_ALL] = {};

	// 魔法の最大同時出現数
	static constexpr int MAX_EFFECT = 10;
};
