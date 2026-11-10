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
#include "GameScene.h"
#include "PauseScene.h"
#include "RankingManager.h"
#include "Random.h"
#include "InputManager.h"


#include "SceneManager.h"
#include "SceneContext.h"

#include "DxLib.h"
#include <memory>


// シーンの種類
enum AppScene {
	APP_TITLE, // タイトルシーン
	APP_GAME,  // ゲームシーン
};

class Game {
public:

	Game();
	void Run(); // ゲームループ

private:

	void Update();
	void Draw();

	// 乱数の本体。各クラスへ注入するため、注入先より先に宣言する
	Random random;
	InputManager inputManager;

	Ground ground;
	Camera camera;
	Player player;
	EnemyManager enemyManager;
	ScoreManager scoreManager;
	ItemManager itemManager;
	FontManager fontManager;
	SoundManager soundManager;
	Result result;
	PauseScene pauseScene;
	RankingManager rankingManager;
	SceneContext context;
	SceneManager sceneManager;

	// 前フレームからの時刻(ミリ秒)
	int prevTime = 0;

	// 前フレームからの経過時間(秒)
	float deltaTime = 0;
};
