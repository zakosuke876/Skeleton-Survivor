#pragma once
#include "Scene.h"
#include "MagicEffectManager.h"

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


// ƒQ[ƒ€“à‚Ìó‘Ô
enum GameState {
	GAME_PLAYING,
	GAME_PAUSE,
	GAME_OVER,
};

class GameScene : public Scene {
public:

	GameScene(Player& player, ScoreManager& scoreManager, ItemManager& itemManager,
			  SoundManager& soundManager, FontManager& fontManager, Ground& ground,
		      PauseScene& pauseScene, Result& result, EnemyManager& enemyManager,
			  RankingManager& rankingManager, Camera& camera);

	void OnEnter() override;
	void OnExit() override;

	SceneType Update(float deltaTime) override;

	void Draw() override;

private:

	// Œ»İ‚ÌƒQ[ƒ€ó‘Ô
	GameState state = GAME_PLAYING;

	Player& player;
	ScoreManager& scoreManager;
	ItemManager& itemManager;
	SoundManager& soundManager;
	FontManager& fontManager;
	Ground& ground;
	Camera& camera;
	PauseScene& pauseScene;
	Result& result;
	EnemyManager& enemyManager;
	RankingManager& rankingManager;
	MagicEffectManager magicEffectManager;
};