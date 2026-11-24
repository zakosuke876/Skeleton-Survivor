#pragma once
#include "Scene.h"
#include "SceneContext.h"

// 前方宣言
class Player;
class ScoreManager;
class ItemManager;
class SoundManager;
class FontManager;
class Ground;
class Camera;
class PauseScene;
class Result;
class EnemyManager;
class RankingManager;
class MagicEffectManager;
class InputManager;
class EffectManager;


// ゲーム内の状態
enum class GameState {
	Playing,
	Pause,
	GameOver,
};

class GameScene : public Scene {
public:

	GameScene(const SceneContext& context);

	void OnEnter() override;
	void OnExit() override;

	SceneType Update(float deltaTime) override;

	void Draw() override;

private:

	// 現在のゲーム状態
	GameState state = GameState::Playing;

	const SoundManager& soundManager;
	const FontManager& fontManager;
	const Ground& ground;
	const Result& result;
	const InputManager& inputManager;
	Player& player;
	ScoreManager& scoreManager;
	ItemManager& itemManager;
	PauseScene& pauseScene;
	EnemyManager& enemyManager;
	RankingManager& rankingManager;
	Camera& camera;
	MagicEffectManager& magicEffectManager;
	EffectManager& effectManager;
};
