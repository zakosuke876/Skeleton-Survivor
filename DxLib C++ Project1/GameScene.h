#pragma once
#include "Player.h"
#include "Camera.h"
#include "ScoreManager.h"
#include "ItemManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "RankingManager.h"
#include "EnemyManager.h"
#include "Ground.h"
#include "PauseScene.h"
#include "Result.h"
#include "Scene.h"

#include "MagicEffect.h"

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
	EnemyManager& enemyManager;
	ScoreManager& scoreManager;
	ItemManager& itemManager;
	SoundManager& soundManager;
	FontManager& fontManager;
	Ground& ground;
	Camera& camera;
	PauseScene& pauseScene;
	Result& result;
	RankingManager& rankingManager;

	MagicEffect magicEffect;
};