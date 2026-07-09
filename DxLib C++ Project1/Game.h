#pragma once
#include "Ground.h"
#include "Camera.h"
#include "Player.h"
#include "EnemyManager.h"
#include "ScoreManager.h"
#include "ItemManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "Result.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "RankingManager.h"
#include "DxLib.h"
#include <random>

// シーンの種類
enum SceneType {
	SCENE_TITLE, // タイトルシーン
	SCENE_GAME,  // ゲームシーン
};

// ゲームの状態
enum GameState {
	GAME_PLAYING, // プレイ中
	GAME_PAUSE,   // ポーズ中
	GAME_OVER,    // ゲームオーバー
};

class Game {
public:

	Game();
	void Run(); // ゲームループ

private:

	void Update();
	void Draw();
	void GameReset(); // ゲームリセット処理

	Ground ground;
	Camera camera;
	Player player;
	EnemyManager enemyManager;
	ScoreManager scoreManager;
	ItemManager itemManager;
	FontManager fontManager;
	SoundManager soundManager;
	Result result;
	TitleScene titleScene;
	PauseScene pauseScene;
	RankingManager rankingManager;

	// 前フレームからの時刻(ミリ秒)
	int prevTime = 0;

	// 前フレームからの経過時間(秒)
	float deltaTime = 0;

	// 現在のシーン
	SceneType currentScene;

	// 現在のゲーム状態
	GameState state = GAME_PLAYING;
};